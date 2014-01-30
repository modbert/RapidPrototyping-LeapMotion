# Learning Objective 2
#
# Code Modified by:
# Michael Odbert
# Vicki Everts
# Nick Aberle
# Jose Vieitez
#
#
#
#
#
################################################################################
# Copyright (C) 2012-2013 Leap Motion, Inc. All rights reserved.               #
# Leap Motion proprietary and confidential. Not for distribution.              #
# Use subject to the terms of the Leap Motion SDK Agreement available at       #
# https://developer.leapmotion.com/sdk_agreement, or another agreement         #
# between Leap Motion and you, your company or other organization.             #
################################################################################

import Leap, sys
from Leap import CircleGesture, KeyTapGesture, ScreenTapGesture, SwipeGesture


class SampleListener(Leap.Listener):
    def on_init(self, controller):
        print "Initialized"

    def on_connect(self, controller):
        print "Connected"

    def on_disconnect(self, controller):
        # Note: not dispatched when running in a debugger.
        print "Disconnected"

    def on_exit(self, controller):
        print "Exited"

    def on_frame(self, controller):
        # Get the most recent frame and report some basic information
        frame = controller.frame()

        #print "Frame id: %d, timestamp: %d, hands: %d, fingers: %d, tools: %d, gestures: %d" % (
        #      frame.id, frame.timestamp, len(frame.hands), len(frame.fingers), len(frame.tools), len(frame.gestures()))

        if not frame.hands.is_empty:
            # Get the first hand
            hand1 = frame.hands[0]
            # Get the Second Hand
            hand2 = frame.hands[1]
            # Get palm position and pass it to function
            get_two_hands_y_position(hand1.palm_position,hand2.palm_position)


# This function prints the height of each hand (max 2)
# It will display the outputs in the right order using the x axis data
def get_two_hands_y_position(hand1,hand2):
    if hand1[0] < hand2[0]:
        print hand1[1],
        print '           ',
        print hand2[1]
        print
        print
        
    else:
        print hand2[1],
        print '           ',
        print hand1[1]
        print
        print


    
def main():
    # Create a sample listener and controller
    listener = SampleListener()
    controller = Leap.Controller()

    # Have the sample listener receive events from the controller
    controller.add_listener(listener)

    # Keep this process running until Enter is pressed
    print "Press Enter to quit..."
    sys.stdin.readline()

    # Remove the sample listener when done
    controller.remove_listener(listener)


if __name__ == "__main__":
    main()
