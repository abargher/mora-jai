# initial firmware notes

[ ] faint "preview" color for corner targets (option)
[ ] servo
    [x] lock function
    [x] unlock function
        - just rotate to 0/90/180 deg
    [ ] store lock/unlock state?? Persistent?
[ ] Filesystem library wrapper?
[ ] report state of pressed buttons
    [x] impl: FW thread polls multiplexer, sends "events" to SW thread when
      button state changes (numbered buttonDown and buttonUp events)
[ ] Update LED colors
    - Q: do we require SW thread to report full matrix state? or allow setting 
    one at a time? or both?
        - can update one at a time, but should also offer an API
          function that updates multiple at once with a single `pixels.show()` call
[ ] Write text/images to screen
    [ ] Q: will we have a more cohesive interface? icons?
    [ ] start with just writing text
[ ] report power/charging/battery % status on screen
[ ] Configuration API
    [ ] persist settings in a file
    [ ] set global LED brightness, including off (quantized options?)
    [ ] turn on/off display
    [ ] set display brightness? (if configurable, see OLED library/docs)
    [ ] query battery percentage
    [ ] query charging status
    [ ] query cable plugged/unplugged status

[ ] Timers????
    [ ] what interface do we need for this? is it all in SW land?
      - probably want a better API for timer functions and interrupts (a la event subscriber below)

[ ] Web server/interface?


- allow game to "subscribe" to update queues
    - game logic provides a function pointer that consumes an event type as input
    - update handler thread awaits events, loops through registered functions and calls them with the event
    - send event API functions that abstract notion of queues (e.g., SetLED(index, color))
