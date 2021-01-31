# stm32-adb2usb [WIP]

A simple implementation of an ADB to USB converter for the STM32F103 'Blue Pill'.
This is work in progress and is not yet functional.
Testing done on an STM32F103 (or rather actually a Chinese clone), but in the end should be compatible with other STM32 microcontrollers/boards.

## Useful resources

I aim to make the code as easy to follow as it is possible in this case, especially the part relating to the ADB protocol.
However, there are some resources that are very useful in understanding how the protocol works, and which I used:

- [ADB - The Untold Story: Space Aliens Ate My Mouse'](https://developer.apple.com/library/archive/technotes/hw/hw_01.html#//apple_ref/doc/uid/DTS10002470) by Apple, the most 'authoritative' source, but lacks some key spec features
- https://web.archive.org/save/http://85.226.187.247/_pdf/Processor_Memory/PIC1617/MIDRANGE/00591A.PDF
- Inside Macintosh volume V, pages 191-192, for the keycodes
- [This blog post](https://www.bigmessowires.com/2016/03/30/understanding-the-adb-service-request-signal/) which explains `Srq` and `Tlt`

