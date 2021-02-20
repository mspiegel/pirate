import * as React from 'react'
// import { LeftTopPosition, XYRange } from '../geometry'

export function useSVGDrag(
    draggedElementRef: React.RefObject<SVGElement>,
    onDragCallback: (left: number, top: number) => void,
): {
    onPointerDown: (event: React.PointerEvent<SVGElement>) => void,
    onPointerMove: (event: React.PointerEvent<SVGElement>) => void,
    onPointerOut: (event: React.PointerEvent<SVGElement>) => void,
    onPointerUp: (event: React.PointerEvent<SVGElement>) => void,
} {
    const [isDragging, setDragging] = React.useState(false)

    const onDragStart = (event: React.PointerEvent<SVGElement>) => {
        event.stopPropagation()
        draggedElementRef.current?.setPointerCapture(event.pointerId)
        setDragging(true)
    }

    function onDrag(event: React.PointerEvent<SVGElement>): void {
        event.stopPropagation()
        if (!isDragging) return
        onDragCallback(event.movementX, event.movementY)
    }

    const onDragEnd = (event: React.PointerEvent<SVGElement>): void => {
        event.stopPropagation()
        if (isDragging) {
            draggedElementRef.current?.releasePointerCapture(event.pointerId)
        }
        setDragging(false)
    }

    return {
        onPointerDown: onDragStart,
        onPointerMove: onDrag,
        onPointerOut: onDragEnd,
        onPointerUp: onDragEnd,
    }

}

// import { XYRange } from '../geometry'

// interface Props {
//     onDragStart: (event: React.PointerEvent<SVGElement>) => LeftTopPosition<number>
//     onDrag: (event: React.PointerEvent<SVGElement>) => LeftTopPosition<number>
//     onDragStop: (event: React.PointerEvent<SVGElement>) => LeftTopPosition<number>
//     readonly dragElementRef: React.RefObject<SVGElement>
//     // readonly height: number
//     readonly initialPosition: LeftTopPosition<number>
//     // readonly left: number
//     // readonly setMyHeight: (height: number) => void
//     // readonly setMyLeft: (left: number) => void
//     // readonly setMyTop: (top: number) => void
//     // readonly setMyWidth: (width: number) => void
//     // readonly top: number
//     // readonly width: number
// }

// interface State {
//     readonly isDragging: boolean
//     readonly left: number
//     readonly top: number
// }

// export class Draggable extends React.Component<Props, State> {

//     #ref: React.RefObject<SVGGElement>

//     constructor(props: Props) {
//         super(props)

//         this.#ref = React.createRef()

//         this.state = {
//             isDragging: false,
//             ...props.initialPosition,
//         }
//     }

//     render(): JSX.Element {

//         const onDragStart = (event: React.PointerEvent<SVGElement>) => {
//             event.stopPropagation()
//             this.props.dragElementRef.current?.setPointerCapture(event.pointerId)
//             this.setState({
//                 isDragging: true,
//                 ...this.props.onDragStart(event),
//             })
//         }

//         const onDrag = (event: React.PointerEvent<SVGGElement>): void => {
//             event.stopPropagation()
//             const { isDragging } = this.state
//             if (!isDragging) return
//             this.setState(this.props.onDrag(event))

//             // const deltaX = event.movementX
//             // const deltaY = event.movementY
//             // this.props.setMyLeft(left + deltaX)
//             // this.props.setMyTop(top + deltaY)
//         }

//         const onDragEnd = (event: React.PointerEvent<SVGGElement>): void => {
//             event.stopPropagation()
//             const { isDragging } = this.state
//             if (isDragging) {
//                 this.props.dragElementRef.current?.releasePointerCapture(event.pointerId)
//             }
//             this.setState({ isDragging: false })
//         }

//         const { onDrag, onDragStart, onDragStop } = this.props

//         return (
//             <g
//                 onPointerDown={onDragStart}
//                 onPointerMove={onDrag}
//                 onPointerOut={onDragStop}
//                 onPointerUp={onDragStop}
//                 ref={this.#ref}
//             // transform={`translate(${left} ${top})`}
//             // y={top}
//             >
//                 {this.props.children}
//             </g>
//         )

//     }

// }
