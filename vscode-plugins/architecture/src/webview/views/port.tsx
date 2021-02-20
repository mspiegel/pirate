/**
 * @module
 * Port view, currently only ever occurring on actors.
 */

import * as React from 'react'

import * as A from '@shared/architecture'

import { Dimensions } from '../geometry'
import * as M from '../mathematics'

import { useSVGDrag } from './draggable'

export const portDimensions = {
    height: 20,
    width: 20,
}

export enum PortDir { Out = 'Out', In = 'In' }

interface Props {
    border: A.Border
    containerDimensions: Dimensions<number>
    direction: PortDir
    name: string
    offset: number
    // setMyBorder: (border: A.Border) => void
    // setMyOffset: (offset: number) => void
}

function getMaxLeft(
    containerDimensions: Dimensions<number>,
    portDimensions: Dimensions<number>,
): number {
    return containerDimensions.width - portDimensions.width
}

function getMaxTop(
    containerDimensions: Dimensions<number>,
    portDimensions: Dimensions<number>,
): number {
    return containerDimensions.height - portDimensions.height
}

export function getPortLocalX(
    border: A.Border,
    containerDimensions: Dimensions<number>,
    offset: number,
    portDimensions: Dimensions<number>,
): number {
    const maxLeft = getMaxLeft(containerDimensions, portDimensions)
    switch (border) {
        case A.Border.Left: return 0
        case A.Border.Right: return maxLeft
        default: return M.clamp(offset, 0, maxLeft)
    }
}

export function getPortLocalY(
    border: A.Border,
    containerDimensions: Dimensions<number>,
    offset: number,
    portDimensions: Dimensions<number>,
): number {
    const maxTop = getMaxTop(containerDimensions, portDimensions)
    switch (border) {
        case A.Border.Top: return 0
        case A.Border.Bottom: return maxTop
        default: return M.clamp(offset, 0, maxTop)
    }
}

function getPortTransform(
    portDimensions: Dimensions<number>,
    props: Props,
): string {
    const { border, containerDimensions, direction, offset } = props

    const x = getPortLocalX(border, containerDimensions, offset, portDimensions)
    const y = getPortLocalY(border, containerDimensions, offset, portDimensions)

    const centerX = portDimensions.width / 2
    const centerY = portDimensions.height / 2

    const rotate = calculatePortRotate(border, direction)
    return `${translateString(x, y)} ${rotateString(rotate, centerX, centerY)}`
}

export function Port(props: Readonly<{
    border: A.Border,
    containerDimensions: Dimensions<number>,
    direction: PortDir,
    name: string,
    offset: number,
    setMyBorder: (border: A.Border) => void,
    setMyOffset: (offset: number) => void,
}>): JSX.Element {

    const rectRef: React.RefObject<SVGGElement> = React.createRef()

    const onDrag = (deltaLeft: number, deltaTop: number) => {
        const maxLeft = props.containerDimensions.width - portDimensions.width
        const maxTop = props.containerDimensions.height - portDimensions.height
        // Calculate x distance above or below max.
        const xDist = outsideRangeDist(deltaLeft, 0, maxLeft)
        // Calculate y distance above or below max.
        const yDist = outsideRangeDist(deltaTop, 0, maxTop)

        // Figuring out which side of the actor rectangle is closest to evt
        const distLeft = euclid2Dist(Math.abs(deltaLeft), yDist)
        const distRight = euclid2Dist(Math.abs(maxLeft - deltaLeft), yDist)
        const distTop = euclid2Dist(Math.abs(deltaTop), xDist)
        const distBottom = euclid2Dist(Math.abs(maxTop - deltaTop), xDist)
        const distMin = Math.min(distLeft, distTop, distRight, distBottom)

        let newBorder: A.Border
        let newOffset: number
        if (distLeft === distMin) {
            newBorder = A.Border.Left
            newOffset = deltaTop
        } else if (distRight === distMin) {
            newBorder = A.Border.Right
            newOffset = deltaTop
        } else if (distTop === distMin) {
            newBorder = A.Border.Top
            newOffset = deltaLeft
        } else {
            newBorder = A.Border.Bottom
            newOffset = deltaLeft
        }

        props.setMyBorder(newBorder)
        props.setMyOffset(newOffset)

    }

    return (
        <g
            {...useSVGDrag(rectRef, onDrag)}
            className="port"
            transform={getPortTransform(portDimensions, props)}
        >
            <rect
                width={portDimensions.width}
                height={portDimensions.height}
                fill='white' stroke='black' strokeWidth='1pt' />
            <path d="M 5 5 L 15 10 L 5 15 z" />
        </g>
    )

    // return (
    //     <use
    //         href='#inPort'
    //         transform={rotateString(rotate, centerX, centerY)}
    //     />
    // )
}

// D.addSVGDragHandlers(this.actorSVG, elt, (evt: D.SVGDragEvent) => {
//             // Maximum left value
//             const maxLeft = this.actorSVG.width.baseVal.value - bbox.width
//             // Maximum top value
//             const maxTop = this.actorSVG.height.baseVal.value - bbox.height
//             // Calculate x distance above or below max.
//             const xDist = outsideRangeDist(evt.left, 0, maxLeft)
//             // Calculate y distance above or below max.
//             const yDist = outsideRangeDist(evt.top, 0, maxTop)

//             // Figuring out which side of the actor rectangle is closest to evt
//             const distLeft = euclid2Dist(Math.abs(evt.left), yDist)
//             const distRight = euclid2Dist(Math.abs(maxLeft - evt.left), yDist)
//             const distTop = euclid2Dist(Math.abs(evt.top), xDist)
//             const distBottom = euclid2Dist(Math.abs(maxTop - evt.top), xDist)
//             const distMin = Math.min(distLeft, distTop, distRight, distBottom)

//             let border: A.Border
//             let offset: number
//             if (distLeft === distMin) {
//                 border = A.Border.Left
//                 offset = evt.top
//             } else if (distRight === distMin) {
//                 border = A.Border.Right
//                 offset = evt.top
//             } else if (distTop === distMin) {
//                 border = A.Border.Top
//                 offset = evt.left
//             } else {
//                 border = A.Border.Bottom
//                 offset = evt.left
//             }
//             // setPortElementPosition(actorSVG, elt, bbox, portType, border, offset)
//             const changes = new ChangeSet()
//             if (this.border !== border) {
//                 // FIXME 'p' here seems wrong
//                 changes.replace(p.border.trackId, border)
//             }
//             if (this.offset !== offset) {
//                 // FIXME 'p' here seems wrong
//                 changes.replace(p.offset.trackId, offset)
//             }
//             sys.sendUpdateDoc(changes)
//         })
//     }

/** `outsideRangeDist(x,l,h)` returns the amount `x` is outside the range `[l,h]`. */
function outsideRangeDist(x: number, l: number, h: number): number {
    if (x < l) { return l - x } else if (x > h) { return h - x } else { return 0 }
}

/**
 * `euclid2Dist(x, y)` returns `sqrt(x * x + y * y)`.
 *
 * It has special cases and assumes `x` and `y` are non-negative.
 */
function euclid2Dist(x: number, y: number) {
    if (x === 0) { return y } else if (y === 0) { return x } else { return Math.sqrt(x * x + y * y) }
}

/**
 * Generate rotate string for a SVG CSS transform attribute.
 */
function rotateString(rotate: number, xoff: number, yoff: number): string {
    return `rotate(${rotate.toString()},${xoff.toString()},${yoff.toString()})`
}

function translateString(x: number, y: number): string {
    return `translate(${x}, ${y})`
}

/**
 * This calculate the orientation of the
 */
function calculatePortRotate(
    border: A.Border,
    dir: PortDir,
): number {
    switch (dir) {
        case PortDir.In: {
            switch (border) {
                case A.Border.Top: return 90
                case A.Border.Right: return 180
                case A.Border.Bottom: return 270
                case A.Border.Left: return 0
                default: return 0
            }
        }
        case PortDir.Out: {
            return (calculatePortRotate(border, PortDir.In) + 180) % 360
        }
    }
}
