/*
 * This work was authored by Two Six Labs, LLC and is sponsored by a subcontract
 * agreement with Galois, Inc.  This material is based upon work supported by
 * the Defense Advanced Research Projects Agency (DARPA) under Contract No.
 * HR0011-19-C-0103.
 *
 * The Government has unlimited rights to use, modify, reproduce, release,
 * perform, display, or disclose computer software or computer software
 * documentation marked with this legend. Any reproduction of technical data,
 * computer software, or portions thereof marked with this legend must also
 * reproduce this marking.
 *
 * Copyright 2020 Two Six Labs, LLC.  All rights reserved.
 */

#include <iomanip>
#include <iostream>
#include "orientationoutput.hpp"

OrientationOutput::OrientationOutput(float angularPositionLimit, bool verbose) :
    AngularPosition(-angularPositionLimit, angularPositionLimit),
    mVerbose(verbose),
    mCallbacks(std::bind(&OrientationOutput::getAngularPosition, this),
               std::bind(&OrientationOutput::setAngularPosition, this,  std::placeholders::_1),
               std::bind(&OrientationOutput::updateAngularPosition, this,  std::placeholders::_1))
{

}

OrientationOutput::~OrientationOutput()
{

}

int OrientationOutput::init()
{
    return 0;
}

void OrientationOutput::term()
{

}

float OrientationOutput::getAngularPosition()
{
    float angularPosition;
    mLock.lock();
    angularPosition = AngularPosition::getAngularPosition();
    mLock.unlock();
    return angularPosition;
}

bool OrientationOutput::setAngularPosition(float& angularPosition)
{
    mLock.lock();

    bool updated = AngularPosition::setAngularPosition(angularPosition);
    if (updated)
    {
        updated = applyAngularPosition(angularPosition);
    }

    if (mVerbose)
    {
        std::cout   << "Camera Position Set "
                    << std::setprecision(4)
                    << angularPosition << std::endl;
    }

    mLock.unlock();
    return updated;
}

bool OrientationOutput::updateAngularPosition(float positionUpdate)
{
    mLock.lock();

    float angularPosition = AngularPosition::getAngularPosition();
    angularPosition += positionUpdate;

    bool updated = AngularPosition::setAngularPosition(angularPosition);

    if (updated)
    {
        updated = applyAngularPosition(angularPosition);
    }

    if (mVerbose)
    {
        std::cout   << "Camera Position Update "
                    << std::setprecision(4) << positionUpdate
                    << " to "
                    << std::setprecision(4) << angularPosition
                    << std::endl;
    }

    mLock.unlock();
    return updated;
}

const CameraOrientationCallbacks& OrientationOutput::getCallbacks()
{
    return mCallbacks;
}

bool OrientationOutput::applyAngularPosition(float angularPosition)
{
    (void) angularPosition;
    return true;
}