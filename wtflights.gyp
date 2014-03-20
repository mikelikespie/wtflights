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
      # This defines interfaces that a device must conform to
      'target_name': 'device',
      'type': 'static_library',
      'dependencies': [
        'util',
      ],
      'sources': [
        "src/device/device.h",
        "src/device/device.cc",
      ],
      'conditions': [
        ['OS=="mac"', {
            'sources': [
              "src/device/sdldevice.h",
              "src/device/sdldevice.cc",
            ]
          },
        ],
      ]
    },
    {
      'target_name': 'wtfsim',
      'type': 'executable',
      'dependencies': [
        'wtflights', 'device'
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
      '-std=c++11',
      '-stdlib=libc++',
    ],
      
    'xcode_settings': {
      'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
    },
  },
  'conditions': [
    ['OS=="mac"', {
        'link_settings': {
        },
      },
    ],
  ],
  'make_global_settings': [
    ['CXX','/usr/bin/clang++'],
    ['LINK','/usr/bin/clang++'],
  ],
}

