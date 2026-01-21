# gather menu files and set up additional dependencies
c = run_command([python, '-c', getFilesCommand.format('.', '*.mnu')])
menuFiles = c.stdout().strip().split('\n')

menuFilesDeps = {}
foreach menu : menuFiles
    if menu == 'main.mnu'
        menuFilesDeps += {menu: []}
    else
        menuFilesDeps += {menu: ['common.mh']}
    endif
endforeach

menuFiles = []
foreach menu, dependencies : menuFilesDeps
    menuFiles += custom_target('mnu2h_@0@'.format(menu),
        input: menu,
        output: '@PLAINNAME@.h',
        depend_files: [dependencies, mnu2hDeps],
        command: [python, mnu2h, '@INPUT@', '@OUTPUT@'],
    )
endforeach
