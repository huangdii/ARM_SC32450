FreeRTOS 튜토리얼
===

## 컴파일 및 실행방법
app 디렉토리내에 원하는 지점으로 이동후 **make clean; make '엔터'**
컴파일 성공 이후,
실습보드의 리셋버튼을 눌러준다

## Source Tree

```shell
├── FreeRTOS
│   ├── Demo
│   │   └── OMAP3_BeagleBoard_GCC
│   ├── License
│   │   └── license.txt
│   ├── Source
│   │   ├── croutine.c
│   │   ├── event_groups.c
│   │   ├── include
│   │   ├── list.c
│   │   ├── portable
│   │   ├── queue.c
│   │   ├── readme.txt
│   │   ├── stream_buffer.c
│   │   ├── tasks.c
│   │   └── timers.c
│   ├── links_to_doc_pages_for_the_demo_projects.url
│   └── readme.txt
├── LICENSE
├── PORT
│   └── fw_beagleboard
│       ├── Exception.c
│       ├── Makefile
│       ├── ParTest.c
│       ├── atmel-ram.ld
│       ├── atmel-rom.ld
│       ├── boot.s
│       ├── keypad.c
│       ├── libs.s
│       ├── macro.h
│       ├── main.c
│       ├── myserial.c
│       ├── omap3-ram.ld
│       ├── omap3.h
│       ├── port.c
│       ├── portISR.c
│       ├── random.s
│       ├── rtosdemo.bin
│       ├── rtosdemo.dis
│       ├── rtosdemo.elf
│       ├── rtosdemo.map
│       ├── serialISR.c
│       └── syscall.c
├── README.md
└── app
    ├── 01_TASKMAN
    │   ├── FreeRTOSConfig.h
    │   ├── Makefile
    │   └── main.c
    ├── 02_STACKOVERFLOW
    │   ├── FreeRTOSConfig.h
    │   ├── Makefile
    │   └── main.c
    ├── 03_CRITICAL
    │   ├── FreeRTOSConfig.h
    │   ├── Makefile
    │   └── main.c
    ├── 04_coROUTINE
    │   ├── FreeRTOSConfig.h
    │   ├── Makefile
    │   └── main.c
    ├── 05_SEM
    │   ├── FreeRTOSConfig.h
    │   ├── Makefile
    │   └── main.c
    ├── 06_COUNT_SEM
    │   ├── FreeRTOSConfig.h
    │   ├── Makefile
    │   ├── keypad.c
    │   └── main.c
    ├── 07_MUTEX
    │   ├── FreeRTOSConfig.h
    │   ├── Makefile
    │   └── main.c
    ├── 08_DEFERRED_INTERRUPT
    │   ├── FreeRTOSConfig.h
    │   ├── Makefile
    │   ├── keypad.c
    │   └── main.c
    ├── 09_FLAG
    │   ├── FreeRTOSConfig.h
    │   ├── Makefile
    │   └── main.c
    ├── 10_SOFT_TIMER
    │   ├── FreeRTOSConfig.h
    │   ├── Makefile
    │   ├── keypad.c
    │   └── main.c
    └── 11_QUE
        ├── FreeRTOSConfig.h
        ├── Makefile
        └── main.c

21 directories, 71 files
```

##Authors

Copyright © 2015 guileschool by Guile Hong (guileschool@gmail.com)

##License

This Source is distributed under the MIT license. Please see the LICENSE
