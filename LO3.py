# Learning Objective 3
#
# Code Modified by:
# Michael Odbert
# Vicki Everts
# Nick Aberle
# Jose Vieitez
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
import pygame
from pygame.locals import *

import math
import numpy


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


def initPyGame():
    size = (1366, 720)

    bits = 16
    #the number of channels specified here is NOT 
    #the channels talked about here http://www.pygame.org/docs/ref/mixer.html#pygame.mixer.get_num_channels

    pygame.mixer.pre_init(44100, -bits, 2)
    pygame.init()
    _display_surf = pygame.display.set_mode(size, pygame.HWSURFACE | pygame.DOUBLEBUF)
    pass

def makeSounds(leftHz, rightHz):
    bits = 16
    duration = 1          # in seconds
    #freqency for the left speaker
    frequency_l = leftHz*2
    #frequency for the right speaker
    frequency_r = rightHz*2

    #this sounds totally different coming out of a laptop versus coming out of headphones

    sample_rate = 44100

    n_samples = int(round(duration*sample_rate))

    #setup our numpy array to handle 16 bit ints, which is what we set our mixer to expect with "bits" up above
    buf = numpy.zeros((n_samples, 2), dtype = numpy.int16)
    max_sample = 2**(bits - 1) - 1

    for s in range(n_samples):
        t = float(s)/sample_rate    # time in seconds

        #grab the x-coordinate of the sine wave at a given time, while constraining the sample to what our mixer is set to with "bits"
        buf[s][0] = int(round(max_sample*math.sin(2*math.pi*frequency_l*t)))        # left
        buf[s][1] = int(round(max_sample*0.5*math.sin(2*math.pi*frequency_r*t)))    # right

    sound = pygame.sndarray.make_sound(buf)
    #play once, then loop forever
    sound.play()
    pass

# This function prints the height of each hand (max 2)
# It will display the outputs in the right order using the x axis data
def get_two_hands_y_position(hand1,hand2):
    if hand1[0] < hand2[0]:
        '''print hand1[1],
        print '           ',
        print hand2[1]
        print
        print'''
        print 'Frequencies:'
        print 'Left:', hand1[1]*2, 'Hz\tRight:', hand2[1]*2, 'Hz'
        makeSounds(hand1[1],hand2[1])
        
    else:
        '''print hand2[1],
        print '           ',
        print hand1[1]
        print
        print'''
        print 'Frequencies:'
        print 'Left:', hand2[1]*2, 'Hz\tRight:', hand1[1]*2, 'Hz'
        makeSounds(hand2[1],hand1[1])


    
def main():
    # Create a sample listener and controller
    listener = SampleListener()
    controller = Leap.Controller()

    # Have the sample listener receive events from the controller
    controller.add_listener(listener)

    # Initialize PyGame utils and window
    initPyGame()

    # Keep this process running until Enter is pressed
    print "Press Enter to quit..."
    sys.stdin.readline()

    # Remove the sample listener when done
    controller.remove_listener(listener)


if __name__ == "__main__":
    main()
