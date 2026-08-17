# initial firmware notes
- faint "preview" color for corner targets (option)
- servo
    - lock function
    - unlock function
    - store lock/unlock state?? Persistent?
- FS library wrapper?
- report state of pressed buttons
    - impl: FW thread polls multiplexer, sends "events" to SW thread when
      button state changes (numbered buttonDown and buttonUp events)
- Update LED colors
    - Q: do we require SW thread to report full matrix state? or allow setting 
    one at a time? or both?
- Write text/images to screen
    - Q: will we have a more cohesive interface? icons?
    - start with just writing text
- report power/charging/battery % status on screen
- Configuration API
    - set global LED brightness, including off (quantized options?)
    - turn on/off display
    - set display brightness? (if configurable, see OLED library/docs)
    - query battery percentage
    - query charging status
    - query cable plugged/unplugged status

- Timers????
    - what interface do we need for this? is it all in SW land?

- Web server/interface?


