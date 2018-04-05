{
    "targets" : [
        {
            "target_name" : "native-image-diff",
            "cflags" : [
                "-Wall",
                "-Wno-unused-parameter",
                "-Wno-missing-field-initializers",
                "-Wextra"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ],
            "sources" : [
                "./native/module.cpp",
                "./native/image-diff.cpp",
                "./native/colors.cpp",
                "./native/antialiasing.cpp",
                "./native/utils.cpp",
            ]
        }
    ]
}
