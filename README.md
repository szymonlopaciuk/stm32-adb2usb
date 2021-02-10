# stm32-adb2usb

A simple implementation of an ADB to USB converter for the STM32F103 'Blue Pill'.
It is currently functional, but there are many ways in which it could be improved upon (see TODO).
Testing done on an STM32F103 (or rather actually a Chinese clone), but in the end should be compatible with other STM32 microcontrollers/boards.
This code is a Platform IO project reliant on ST's stm32duino, although I am sure it could be easily converted to something else.

## Useful resources

I aim to make the code as easy to follow as it is possible in this case, especially the part relating to the ADB protocol.
By far the best and most comprehensive description of the ADB protocol and devices is Apple's [Guide to the Macintosh&copy; Family Hardware (2nd ed.)](https://archive.org/details/apple-guide-macintosh-family-hardware) (chapter 8, p. 287).
Some other resources that were useful to me as well:

- [ADB - The Untold Story: Space Aliens Ate My Mouse'](https://developer.apple.com/library/archive/technotes/hw/hw_01.html#//apple_ref/doc/uid/DTS10002470) by Apple, quick introduction to what ADB is about, but lacks some key spec features
- https://web.archive.org/save/http://85.226.187.247/_pdf/Processor_Memory/PIC1617/MIDRANGE/00591A.PDF
- Inside Macintosh volume V, pages 191-192, for keycodes
- [This blog post](https://www.bigmessowires.com/2016/03/30/understanding-the-adb-service-request-signal/) which explains `Srq` and `Tlt`

## TODO

- [x] Make the keyboard work
- [ ] Fix timers, using `micros()` for time differences will cause problems
- [ ] Make the mouse work
- [ ] Ditch bitfields as the ordering of bytes/bits is confusing and not super portable (see tests failing; passing around uint16_t and then casting to bitfield structs is really dodgy and I shouldn't be doing that)
- [ ] Handle SRQs and multiple devices of the same type properly
- [ ] Make the layout easier to customise (swap a control, or num lock, for fn?)
