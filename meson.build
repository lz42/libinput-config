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
		dependency('libinput'),
		dependency('libudev')
	],
	
	install: true
)

libdir = get_option('prefix') / get_option('libdir')
project_name = meson.project_name()

if get_option('non_glibc')
	configure_file(
		input: 'scripts/profile.sh.in',
		output: 'libinput-config.sh',
		
		configuration: {
			'libdir': libdir,
			'project_name': project_name
		},
		
		install: true,
		install_dir: '/etc/profile.d'
	)
else
	meson.add_install_script('scripts/preload.sh', libdir, project_name)
endif
