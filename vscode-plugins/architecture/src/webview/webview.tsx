/**
 * @module
 * Sets up the webview component of the plug-in, and gives the implementation
 * of the system services.
 */

import { Context } from 'immutability-helper'
import * as React from 'react'
import * as ReactDOM from 'react-dom'

import * as A from '@shared/architecture'
import { common, extension, webview } from '@shared/webviewProtocol'

import { ChangeSet, TrackedIndex } from './changeSet'
import { Rectangle, XRange, XYRange, YRange } from './geometry'
import * as svg from './svg'
import { SystemServices } from './systemServices'
import { Actor } from './views/actor'
import { Bus } from './views/bus'
import { Connectable, Connection } from './views/connection'
import { getPortLocalX, getPortLocalY, portDimensions } from './views/port'


const update = (new Context()).update

type TrackedValueListener = (newValue: string) => void

function svgSetLength(e: SVGAnimatedLength, l: A.Length) {
    switch (l.units) {
        case A.Units.CM:
            svg.setCmUnits(e, l.value)
            break
        case A.Units.IN:
            svg.setInchUnits(e, l.value)
            break
    }
}

interface Props {
    sys: Webview
}

interface State {
    actors: A.TrackedValue<A.Actor>[]
    buses: A.TrackedValue<A.Bus>[]
    connections: A.Connection[]
}

function getXYRange(o: XYRange<A.TrackedValue<number>>): XYRange<number> {
    return {
        height: o.height.value,
        left: o.left.value,
        top: o.top.value,
        width: o.width.value,
    }
}

class WebviewComponent extends React.Component<Props, State> {

    #svgRef: React.RefObject<SVGSVGElement>

    constructor(props: Props) {
        super(props)

        this.#svgRef = React.createRef()

        this.state = {
            actors: [],
            buses: [],
            connections: [],
        }

        this.onMessage = this.onMessage.bind(this)
    }

    componentDidMount() {
        window.addEventListener('message', this.onMessage)
    }

    componentWillUnmount() {
        window.removeEventListener('message', this.onMessage)
    }

    getCollidables(): readonly XYRange<number>[] {
        return ([] as readonly XYRange<number>[]).concat(
            this.state.actors.map(a => getXYRange(a.value)),
            this.state.buses.map(b => getXYRange(b.value)),
        )
    }

    getConnectables(
        connection: A.Connection,
    ): { source: Connectable, target: Connectable } | undefined {
        const source = this.getEndpoint(connection.source)
        if (!source) { return }
        const target = this.getEndpoint(connection.target)
        if (!target) { return }
        return { source, target }
    }

    getEndpoint(
        endpoint: A.Endpoint,
    ): Connectable | undefined {
        switch (endpoint.type) {
            case A.EndpointType.Port: {
                const actor = this.state.actors.find(a =>
                    a.value.name.value === endpoint.actor
                )
                if (!actor) { return }
                const allPorts = ([] as readonly A.Port[]).concat(
                    actor.value.inPorts.map(p => p.value),
                    actor.value.outPorts.map(p => p.value),
                )
                const port = allPorts.find(p => p.name.value === endpoint.port)
                if (!port) { return }
                return { actor: actor.value, port: port }
            }
            case A.EndpointType.Bus: {
                return this.state.buses.find(b => b.value.name.value === endpoint.bus)?.value
            }
        }
    }

    getEndpointKey(
        endpoint: A.Endpoint,
    ): string {
        switch (endpoint.type) {
            case A.EndpointType.Port: return `${endpoint.actor}#${endpoint.port}`
            case A.EndpointType.Bus: return `${endpoint.bus}`
        }
    }

    onMessage(event: MessageEvent) {
        // The JSON data that the extension sent
        const message = event.data as extension.Event
        // console.log(message)

        switch (message.tag) {
            case extension.Tag.SetSystemModel: {
                console.log('SetSystemModel')

                const sys = message.system

                // TODO: make this part of the state
                const svg = this.#svgRef.current
                if (svg) {
                    svgSetLength(svg.width, sys.pagewidth)
                    svgSetLength(svg.height, sys.pageheight)
                    svg.height.baseVal.convertToSpecifiedUnits(svg.width.baseVal.unitType)
                }

                const s = message.system
                // type annotation guarantees we don't forget new fields
                const newState: State = {
                    actors: s.actors,
                    buses: s.buses,
                    connections: s.connections,
                }
                this.setState(newState)

                this.props.sys.setSystemModelDone()
                break
            }
            case extension.Tag.InvalidateModel: {
                console.log('InvalidateModel')
                // type annotation guarantees we don't forget new fields
                const newState: State = {
                    actors: [],
                    buses: [],
                    connections: [],
                }
                this.setState(newState)
                this.props.sys.setSystemModelDone()
                break
            }
            case extension.Tag.DocumentEdited: {
                this.props.sys.documentEdited(message.edits)
                break
            }
        }
    }

    render(): JSX.Element {

        // sets a field whose type is <A.TrackedValue<T>
        const setField =
            <ValueType, R extends Record<Key, A.TrackedValue<ValueType>>, Key extends keyof R>
                (fieldName: Key) =>
                (fieldValue: ValueType) =>
                    ({ [fieldName]: { value: { $set: fieldValue } } })

        const setFieldAtIndex =
            <ValueType, R extends Record<Key, A.TrackedValue<ValueType>>, Key extends keyof R>
                (fieldName: Key, index: number) =>
                (fieldValue: ValueType) =>
                    ({ [index]: { value: setField<ValueType, R, Key>(fieldName)(fieldValue) } })

        // type trick to talk about the type of elements of an array type
        // type ArrayElement<ArrayType extends readonly unknown[]> =
        //     ArrayType extends readonly (infer ElementType)[] ? ElementType : never

        /**
         * Generic function for setting a field value within the state.
         * Calls look like:
         *
         *   setStateField('actors', actor, index, 'width')
         *
         */
        const setStateField =
            <
                ModelKey extends keyof SubModel & keyof State,
                SubModel extends State & Record<ModelKey, TrackedObjectType[]>,
                ValueType extends number | string,
                FieldKey extends keyof TrackedObjectType['value'],
                TrackedFieldType extends A.TrackedValue<ValueType>,
                TrackedObjectType extends A.TrackedValue<Record<FieldKey, TrackedFieldType>>
            >(
                modelKey: ModelKey,
                // this is currently unused but helps type inference...
                // TODO: find a way to get rid of it
                _trackedObject: TrackedObjectType,
                index: number,
                fieldKey: FieldKey,
            ) =>
                (value: ValueType) => {

                    // immediately set the state to avoid the roundtrip lag
                    this.setState((prevState: State) => {
                        return ({
                            [modelKey]: update(
                                prevState[modelKey],
                                setFieldAtIndex<
                                    // eslint-disable-next-line @typescript-eslint/no-explicit-any
                                    any, // ValueType,
                                    // eslint-disable-next-line @typescript-eslint/no-explicit-any
                                    any, // Record<ValueKey, A.TrackedValue<ValueType>>,
                                    // eslint-disable-next-line @typescript-eslint/no-explicit-any
                                    any // ValueKey
                                // eslint-disable-next-line @typescript-eslint/no-explicit-any
                                >(fieldKey, index)(value) as any,
                            ),
                        } as Pick<State, ModelKey>)
                    })
                }

        const broadcastUpdate =
            <
                ModelKey extends keyof SubModel & keyof State,
                SubModel extends State & Record<ModelKey, TrackedObjectType[]>,
                ValueType extends number | string,
                FieldKey extends keyof TrackedObjectType['value'],
                TrackedFieldType extends A.TrackedValue<ValueType>,
                TrackedObjectType extends A.TrackedValue<Record<FieldKey, TrackedFieldType>>
            >(
                trackedObject: TrackedObjectType,
                fieldKey: FieldKey,
            ) =>
                (value: ValueType) => {
                    // but also indicate the change to the document
                    const changes = new ChangeSet()
                    changes.replace(trackedObject.value[fieldKey].trackId, value)
                    this.props.sys.sendUpdateDoc(changes)
                }

        const updateStateAndBroadcast =
            <
                ModelKey extends keyof SubModel & keyof State,
                SubModel extends State & Record<ModelKey, TrackedObjectType[]>,
                ValueType extends number | string,
                FieldKey extends keyof TrackedObjectType['value'],
                TrackedFieldType extends A.TrackedValue<ValueType>,
                TrackedObjectType extends A.TrackedValue<Record<FieldKey, TrackedFieldType>>
            >(
                modelKey: ModelKey,
                trackedObject: TrackedObjectType,
                index: number,
                fieldKey: FieldKey,
            ) =>
                (value: ValueType) => {
                    setStateField(modelKey, trackedObject, index, fieldKey)(value)
                    broadcastUpdate(trackedObject, fieldKey)(value)
                }

        const listenToTrackedInt =
            <
                FieldKey extends keyof TrackedObjectType['value'],
                TrackedFieldType extends A.TrackedValue<number>,
                TrackedObjectType extends A.TrackedValue<Record<FieldKey, TrackedFieldType>>,
                ModelKey extends keyof SubModel & keyof State,
                SubModel extends Record<ModelKey, TrackedObjectType[]>
            >
                (
                    objectKey: ModelKey,
                    trackedObject: TrackedObjectType,
                    index: number,
                    fieldKey: FieldKey,
            ): void => {
                this.props.sys.whenIntTrackChanged(
                    trackedObject.value[fieldKey].trackId,
                    setStateField(objectKey, trackedObject, index, fieldKey)
                )
            }

        // Register listeners for tracked actor fields
        this.state.actors.map((actor, index) => {
            listenToTrackedInt('actors', actor, index, 'height')
            listenToTrackedInt('actors', actor, index, 'left')
            listenToTrackedInt('actors', actor, index, 'top')
            listenToTrackedInt('actors', actor, index, 'width')
            // TODO: ports
        })

        // Register listeners for tracked bus fields
        this.state.buses.map((bus, index) => {
            listenToTrackedInt('buses', bus, index, 'height')
            listenToTrackedInt('buses', bus, index, 'left')
            listenToTrackedInt('buses', bus, index, 'top')
            listenToTrackedInt('buses', bus, index, 'width')
        })

        // TODO: connections

        const actors = this.state.actors.map((trackedActor, index) => {
            const actor = trackedActor.value
            return (
                <Actor
                    color={actor.color.value}
                    height={actor.height.value}
                    inPorts={actor.inPorts}
                    left={actor.left.value}
                    location={actor.location}
                    name={actor.name.value}
                    outPorts={actor.outPorts}
                    key={actor.name.value}
                    setMyHeight={updateStateAndBroadcast('actors', trackedActor, index, 'height')}
                    setMyLeft={updateStateAndBroadcast('actors', trackedActor, index, 'left')}
                    setMyTop={updateStateAndBroadcast('actors', trackedActor, index, 'top')}
                    setMyWidth={updateStateAndBroadcast('actors', trackedActor, index, 'width')}
                    sys={this.props.sys}
                    top={actor.top.value}
                    width={actor.width.value}
                />
            )
        })

        const buses = this.state.buses.map((bus, index) =>
            <Bus
                height={bus.value.height.value}
                left={bus.value.left.value}
                key={bus.value.name.value}
                locatedBus={bus}
                setMyHeight={updateStateAndBroadcast('buses', bus, index, 'height')}
                setMyLeft={updateStateAndBroadcast('buses', bus, index, 'left')}
                setMyTop={updateStateAndBroadcast('buses', bus, index, 'top')}
                setMyWidth={updateStateAndBroadcast('buses', bus, index, 'width')}
                sys={this.props.sys}
                top={bus.value.top.value}
                width={bus.value.width.value}
            />
        )

        // FIXME: the logic in here is a bit gnarly, try to improve it
        const connections = this.state.connections.flatMap(c => {
            const connectables = this.getConnectables(c)
            if (!connectables) return []

            let actor: A.Actor | undefined = undefined
            let bus: A.Bus | undefined = undefined
            let port: A.Port | undefined = undefined

            // Assumption: one endpoint is an actor port, the other a bus

            if (c.source.type === A.EndpointType.Port) {
                actor = (connectables.source as { actor: A.Actor }).actor
                port = (connectables.source as { port: A.Port }).port
            }
            if (c.target.type === A.EndpointType.Port) {
                actor = (connectables.target as { actor: A.Actor }).actor
                port = (connectables.target as { port: A.Port }).port
            }

            if (c.source.type === A.EndpointType.Bus) {
                bus = connectables.source as A.Bus
            }
            if (c.target.type === A.EndpointType.Bus) {
                bus = connectables.target as A.Bus
            }

            if (!actor || !bus || !port) { return [] }

            const busLeft = bus.left.value
            const busRight = busLeft + bus.width.value
            const busTop = bus.top.value
            const busBottom = busTop + bus.height.value

            const portX = actor.left.value + getPortLocalX(
                port.border.value,
                { height: actor.height.value, width: actor.width.value },
                port.offset.value,
                portDimensions,
            ) + portDimensions.width / 2
            const portY = actor.top.value + getPortLocalY(
                port.border.value,
                { height: actor.height.value, width: actor.width.value },
                port.offset.value,
                portDimensions,
            ) + portDimensions.height / 2

            const sourceKey = this.getEndpointKey(c.source)
            const targetKey = this.getEndpointKey(c.target)

            return [
                <Connection
                    busBottom={busBottom}
                    busLeft={busLeft}
                    busOrientation={bus.orientation}
                    busRight={busRight}
                    busTop={busTop}
                    key={`${sourceKey}-${targetKey}`}
                    portX={portX}
                    portY={portY}
                />,
            ]
        })

        const makeListItem = (
            description: string,
            value: JSX.Element,
        ): JSX.Element => {
            return <li style={{ width: '100%' }}>
                <span>{description}</span>
                <span style={{ float: 'right' }}>{value}</span>
            </li>
        }

        const busesList = this.state.buses.map((locatedBus, _index) => {
            const bus = locatedBus.value

            return (
                <li key={bus.name.value}>
                    <details style={{ padding: 0 }}>
                        <summary>{bus.name.value}</summary>
                        <ul style={{ listStyle: 'none', padding: '2px' }}>
                            {makeListItem(
                                'Left x Top:',
                                <span>{bus.left.value} x {bus.top.value}</span>
                            )}
                            {makeListItem(
                                'Width x Height:',
                                <span>{bus.width.value} x {bus.height.value}</span>
                            )}
                            {makeListItem('Orientation:', <span>{bus.orientation}</span>)}
                        </ul>
                    </details>
                </li>
            )

        })

        const actorsList = this.state.actors.map((locatedActor, _index) => {
            const actor = locatedActor.value

            const inPorts = actor.inPorts.map((locatedPort) => {
                const port = locatedPort.value
                return <li key={port.name.value}>
                    {port.name.value}
                </li>
            })

            const outPorts = actor.outPorts.map((locatedPort) => {
                const port = locatedPort.value
                return <li key={port.name.value}>
                    {port.name.value}
                </li>
            })

            return (
                <li key={actor.name.value}>
                    <details style={{ padding: 0 }}>
                        <summary>{actor.name.value}</summary>
                        <ul style={{ listStyle: 'none', padding: '2px' }}>
                            {makeListItem(
                                'Left x Top:',
                                <span>{actor.left.value} x {actor.top.value}</span>
                            )}
                            {makeListItem(
                                'Width x Height:',
                                <span>{actor.width.value} x {actor.height.value}</span>
                            )}
                            {makeListItem('Color:', <span>{actor.color.value}</span>)}
                            <li>In ports:
                                <ul style={{ listStyle: 'none', padding: '2px' }}>
                                    {inPorts}
                                </ul>
                            </li>
                            <li>Out ports:
                                <ul style={{ listStyle: 'none', padding: '2px' }}>
                                    {outPorts}
                                </ul>
                            </li>
                        </ul>
                    </details>
                </li>
            )
        })

        // order matters: later will appear on top
        return (
            <span>
                <span
                    style={{
                        backgroundColor: 'var(--vscode-sideBar-background',
                        bottom: 0,
                        left: 0,
                        overflowX: 'auto',
                        position: 'absolute',
                        right: '2in',
                        top: 0,
                    }}
                >
                    <svg ref={this.#svgRef}>
                        {connections}
                        {buses}
                        {actors}
                    </svg>
                </span>
                <span
                    id="controls"
                    style={{
                        backgroundColor: 'darkgray',
                        bottom: 0,
                        color: 'black',
                        margin: '10px',
                        overflowY: 'clip',
                        position: 'absolute',
                        right: 0,
                        top: 0,
                        width: '2in',
                    }}
                >
                    <div id="lasterrormsg"></div>
                    <p style={{ margin: '2px' }}>Actors</p>
                    <ul style={{ listStyle: 'none', margin: '2px', padding: '2px' }}>
                        {actorsList}
                    </ul>
                    <p style={{ margin: '2px' }}>Buses</p>
                    <ul style={{ listStyle: 'none', margin: '2px', padding: '2px' }}>
                        {busesList}
                    </ul>
                </span>
            </span>
        )

    }

}

/**
 *  Class that manages webview state.
 */
class Webview implements SystemServices {

    #errormsgDiv = document.getElementById('lasterrormsg') as HTMLDivElement

    constructor() {
        return
    }

    setSystemModel(): void {
        console.log('setSystemModel, ignoring')
    }

    clearModel(errorMsgText: string): void {
        if (this.#errormsgDiv) { this.#errormsgDiv.innerText = errorMsgText }
    }

    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    adjustX(thisObject: any, r: YRange<number>, width: number, oldLeft: number, newLeft: number): number {
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
        const collidables: any = []

        const top = r.top
        const bottom = top + r.height

        // If we move left
        if (newLeft < oldLeft) {
            for (const collidable of collidables) {
                if (collidable === thisObject) continue
                if (bottom <= collidable.top) continue
                if (collidable.bottom <= top) continue
                // If o is left of oldLeft and right of newLeft
                if (collidable.right <= oldLeft) { newLeft = Math.max(collidable.right, newLeft) }
            }
        }

        // If we move right
        if (newLeft > oldLeft) {
            const oldRight = oldLeft + width
            let newRight = newLeft + width
            for (const collidable of collidables) {
                if (collidable === thisObject) continue
                if (bottom <= collidable.top) continue
                if (collidable.bottom <= top) continue
                // If o is right of oldRight and left of newRight.
                if (oldRight <= collidable.left) { newRight = Math.min(newRight, collidable.left) }
            }
            newLeft = newRight - width
        }

        return newLeft
    }

    adjustY(thisActor: unknown, r: XRange<number>, height: number, oldTop: number, newTop: number): number {
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
        const l: any = []
        const left = r.left
        const right = left + r.width

        // If we move up
        if (newTop < oldTop) {
            for (const o of l) {
                if (o === thisActor) continue
                if (right <= o.left) continue
                if (o.right <= left) continue
                // If o is above of oldTop
                if (o.bottom <= oldTop) { newTop = Math.max(o.bottom, newTop) }
            }
        }

        // If we move right
        if (newTop > oldTop) {
            const oldBottom = oldTop + height
            let newBottom = newTop + height
            for (const o of l) {
                if (o === thisActor) continue
                if (right <= o.left) continue
                if (o.right <= left) continue
                // If o is right of oldRight and left of newRight.
                if (oldBottom <= o.top) { newBottom = Math.min(newBottom, o.top) }

            }
            newTop = newBottom - height
        }

        return newTop
    }

    overlaps(thisObject: unknown, r: Rectangle<number>): boolean {
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
        const collidables: any[] = []
        for (const collidable of collidables) {
            if (collidable === thisObject) continue
            if (r.right <= collidable.left) continue
            if (collidable.right <= r.left) continue
            if (r.bottom <= collidable.top) continue
            if (collidable.bottom <= r.top) continue
            return true
        }
        return false
    }

    private sendToExtension(r: webview.Event): void {
        vscode.postMessage(r)
    }

    visitURI(idx: A.LocationIndex): void {
        const cmd: webview.VisitURI = {
            tag: webview.Tag.VisitURI,
            locationIdx: idx,
        }
        this.sendToExtension(cmd)
    }

    setSystemModelDone(): void {
        const upd: webview.SetSystemModelDone = { tag: webview.Tag.SetSystemModelDone }
        this.sendToExtension(upd)
    }

    sendUpdateDoc(s: ChangeSet) {
        if (s.edits.length > 0) {
            const msg: webview.UpdateDocument = {
                tag: webview.Tag.UpdateDocument,
                edits: s.edits,
            }
            this.sendToExtension(msg)
        }
    }

    #trackListeners: Map<TrackedIndex, TrackedValueListener> = new Map()

    /* Returns the listener for that index. */
    getListener(idx: TrackedIndex): TrackedValueListener {
        const listener = this.#trackListeners.get(idx)
        if (!listener) { return () => { return } }
        return listener
    }

    whenTrackedChanged<V>(
        idx: TrackedIndex,
        parser: (newValue: string) => V | undefined,
        listener: (newValue: V) => void,
    ): void {
        const handler = (newValue: string) => {
            const parsed = parser(newValue)
            if (parsed) { listener(parsed) }
        }
        this.#trackListeners.set(idx, handler)
    }

    whenStringTrackChanged(idx: TrackedIndex, listener: (newValue: string) => void): void {
        this.whenTrackedChanged(idx, s => s, listener)
    }

    whenIntTrackChanged(idx: TrackedIndex, listener: (newValue: number) => void): void {
        this.whenTrackedChanged(
            idx,
            (newValue) => {
                const i = parseInt(newValue)
                if (isNaN(i)) {
                    console.log(`Received ${newValue} when int expected.`)
                    return
                }
                return i
            },
            listener
        )
    }

    documentEdited(edits: readonly common.TrackUpdate[]) {
        for (const e of edits) {
            const listener = this.getListener(e.trackIndex)
            listener(e.newText)
        }
    }

}

// eslint-disable-next-line @typescript-eslint/no-explicit-any
declare const acquireVsCodeApi: any

const vscode = acquireVsCodeApi()

{
    const container = (document.getElementById('webview-container') as unknown) as HTMLDivElement
    if (container === null) {
        console.log('Webview container <div> is missing')
    } else {
        const sys = new Webview()

        ReactDOM.render(
            <WebviewComponent sys={sys} />,
            container,
        )

        console.log('Start loop')

        // window.addEventListener('message', event => {
        //     const message = event.data as extension.Event // The json data that the extension sent
        //     switch (message.tag) {
        //         case extension.Tag.SetSystemModel:
        //             sys.setSystemModel(message.system)
        //             sys.setSystemModelDone()
        //             break
        //         case extension.Tag.InvalidateModel:
        //             sys.clearModel('Invalid model.')
        //             sys.setSystemModelDone()
        //             break
        //         case extension.Tag.DocumentEdited:
        //             sys.documentEdited(message.edits)
        //             break
        //     }
        // })
        vscode.postMessage({ tag: webview.Tag.Ready })
    }
}
