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

configure_file(
	input: 'profile.sh.in',
	output: 'libinput-config.sh',
	
	configuration: {
		'libdir': get_option('prefix') / get_option('libdir'),
		'project_name': meson.project_name()
	},
	
	install: true,
	install_dir: '/etc/profile.d'
)
