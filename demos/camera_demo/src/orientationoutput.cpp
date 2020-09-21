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

bool OrientationOutput::setAngularPosition(float angularPosition)
{
    mLock.lock();
    if (mVerbose)
    {
        std::cout   << "Camera Position "
                    << std::setprecision(4)
                    << angularPosition << std::endl;
    }

    bool updated = AngularPosition::setAngularPosition(angularPosition);
    mLock.unlock();
    return updated;
}

const CameraOrientationCallbacks& OrientationOutput::getCallbacks()
{
    return mCallbacks;
}