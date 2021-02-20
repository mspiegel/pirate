/**
 * @module
 * Connection view.  A connection is displayed as a link connection its
 * endpoints.  As such, it has no "proper" coordinates, and must instead rely on
 * staying up-to-date with the coordinates of the endpoints it links.
 */

import * as React from 'react'

import * as A from '@shared/architecture'

import * as M from '../mathematics'

export type Connectable = A.Bus | { actor: A.Actor, port: A.Port }

interface Props {
    busOrientation: A.BusOrientation
    busLeft: number
    busTop: number
    busRight: number
    busBottom: number
    portX: number
    portY: number
}

// eslint-disable-next-line @typescript-eslint/no-empty-interface
interface State {
}

export class Connection extends React.Component<Props, State> {
    constructor(props: Props) {
        super(props)
    }

    render(): JSX.Element {

        const { busBottom, busLeft, busOrientation, busRight, busTop } = this.props
        const { portX, portY } = this.props

        // always copy the port XY
        const x1 = portX
        const y1 = portY

        // try to reach the bus perpendicularly, if possible
        const x2 = (
            busOrientation === A.BusOrientation.Horizontal
            ? M.clamp(portX, busLeft, busRight)
            : (portX <= busLeft ? busLeft : busRight)
        )
        const y2 = (
            busOrientation === A.BusOrientation.Horizontal
            ? (portY <= busTop ? busTop : busBottom)
            : M.clamp(portY, busTop, busBottom)
        )

        return (
            <line
                className='connection'
                x1={x1}
                x2={x2}
                y1={y1}
                y2={y2}
            />
        )
    }
}
