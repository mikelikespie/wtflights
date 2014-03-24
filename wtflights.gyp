{  
  'targets': [
    {
      'target_name': 'util',
      'type': 'static_library',
      'sources': [
        "src/util/geometry.h",
        "src/util/geometry.cc",
      ],
      'link_settings': {
        'linkflags': [
        ],
      },
    },
    {
      'target_name': 'wtflights',
      'type': 'static_library',
      'dependencies': [
        'util'
      ],
      'sources': [
        "src/wtflights/wtflights.cc",
      ],
    },
    {
      'target_name': 'device',
      'type': 'static_library',
      'dependencies': [
        'util',
      ],
      'sources': [
        "src/device/device.h",
        "src/device/device.cc",
      ],
    },
    {
      'target_name': 'sdldevice',
      'type': 'static_library',
      'dependencies': [
        'device',
      ],
      'sources': [
        "src/device/sdldevice.h",
        "src/device/sdldevice.cc",
      ],
    },
    {
      'target_name': 'cxxabishim',
      'sources': [
        "src/cxxabishim/unwind.c",
      ],
      'type': 'shared_library',
      'link_settings': {
        'libraries': [
          '-lunwind',
          '-lc++',
        ],
      },
    },
    {
      'target_name': 'ledscapedevice',
      'type': 'static_library',
      'dependencies': [
        'device',
        'cxxabishim',
        'LEDscape/ledscape.gyp:ledscape',
      ],
      'sources': [
        "src/device/ledscapedevice.h",
        "src/device/ledscapedevice.cc",
      ],
      'link_settings': {
        'libraries': [
          '-lc++',
          '-lc++abi',
        ],
      },
    },
    {
      'target_name': 'wtfsim',
      'type': 'executable',
      'dependencies': [
        'wtflights',
        'device',
        'sdldevice',
      ],
      'sources': [
        "src/wtfsim/wtfsim.cc",
      ],
      'link_settings': {
        'libraries': [
          'libsdl2.dylib',
          'OpenGL.framework',
          'System.framework',
          'CoreFoundation.framework',
          'CoreGraphics.framework',
          'CoreText.framework',
        ],
      },
    },
    {
      'target_name': 'wtfrunner',
      'type': 'executable',
      'dependencies': [
        'wtflights',
        'device',
        'ledscapedevice',
      ],
      'sources': [
        "src/wtfrunner/wtfrunner.cc",
      ],
      'link_settings': {
      },
    },
  ],
  'target_defaults': {
    'include_dirs': [
      '/usr/local/include',
      'src',
    ],

    'link_settings': {
      'library_dirs': [
        '/usr/lib/system',
        '/usr/local/lib',
      ],
    },
    'cflags': [
      '-O0',
      '-g',
    ],
    'cflags_cc': [
      '-std=c++11',
      '-stdlib=libc++',
    ],
      
    'xcode_settings': {
      'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
    },
  },
  'make_global_settings': [
    ['CC','/usr/bin/clang'],
    ['CXX','/usr/bin/clang++'],
    ['LINK','/usr/bin/clang++'],
  ],
}

