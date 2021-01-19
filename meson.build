project('libinput-config', 'c')

cc = meson.get_compiler('c')

shared_library('input-config',
	[
		'src/hook.c',
		'src/keyfile.c',
		'src/main.c'
	],
	
	dependencies: [
		cc.find_library('dl', required: false),
		dependency('libinput')
	],
	
	install: true
)

meson.add_install_script('preload.sh',
	get_option('prefix') / get_option('libdir'),
	meson.project_name()
)
