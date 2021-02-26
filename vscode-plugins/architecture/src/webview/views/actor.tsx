/**
 * @module
 * Actor view, responsible for displaying itself and its ports.
 */

import * as React from 'react'

import * as A from '@shared/architecture'

import { SystemServices } from '../systemServices'

import { useSVGDrag } from './draggable'
import { Port, PortDir } from './port'

export function Actor(props: Readonly<{
    color: string
    height: number
    inPorts: readonly A.TrackedValue<A.Port>[]
    left: number
    location: A.LocationIndex
    name: string
    outPorts: readonly A.TrackedValue<A.Port>[]
    setMyHeight: (height: number) => void
    setMyLeft: (left: number) => void
    setMyTop: (top: number) => void
    setMyWidth: (width: number) => void
    sys: SystemServices
    top: number
    width: number
}>): JSX.Element {

    const containerDimensions = {
        height: props.height,
        width: props.width,
    }

    const foreignObjectRef: React.RefObject<SVGForeignObjectElement> = React.createRef()

    const inPorts = props.inPorts.map(({ value }) =>
        <Port
            border={value.border.value}
            containerDimensions={containerDimensions}
            direction={PortDir.In}
            key={value.name.value}
            name={value.name.value}
            offset={value.offset.value}
            setMyBorder={() => console.log('todo')}
            setMyOffset={() => console.log('todo')}
        />
    )

    const outPorts = props.outPorts.map(({ value }) =>
        <Port
            border={value.border.value}
            containerDimensions={containerDimensions}
            direction={PortDir.Out}
            key={value.name.value}
            name={value.name.value}
            offset={value.offset.value}
            setMyBorder={() => console.log('todo')}
            setMyOffset={() => console.log('todo')}
        />
    )

    const onVisitClassClick = () => {
        props.sys.visitURI(props.location)
    }

    const rect = (
        <rect
            className='enclave'
            fill={props.color}
            height={props.height}
            width={props.width}
        />
    )

    const foreignObject = (
        <foreignObject
            height={props.height}
            ref={foreignObjectRef}
            width={props.width}
        >
            <div className='enclave-content'>
                <span className='enclave-name'>{props.name}</span>
                <a
                    className='enclave-visit-class'
                    onMouseDown={onVisitClassClick}
                >[Visit Class]</a>
            </div>
        </foreignObject>
    )

    const onDrag = (deltaLeft: number, deltaTop: number) => {
        props.setMyLeft(props.left + deltaLeft)
        props.setMyTop(props.top + deltaTop)
    }

    return (
        <g
            {...useSVGDrag(foreignObjectRef, onDrag)}
            transform={`translate(${props.left}, ${props.top})`}
        >
            {rect}
            {foreignObject}
            {inPorts}
            {outPorts}
        </g>
    )

}
