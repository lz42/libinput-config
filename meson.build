project('libinput-config', 'c')

cc = meson.get_compiler('c')

libdir = get_option('prefix') / get_option('libdir')
project_name = meson.project_name()

if get_option('shitty_sandboxing')
	add_project_arguments(
		'-DLIBINPUT_SHITTY_SANDBOXING=1',
		language: 'c'
	)
	
	libdir = '/etc'
endif

if get_option('non_glibc')
	add_project_arguments('-DLIBINPUT_NON_GLIBC=1', language: 'c')
	
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
	
	configure_file(
		input: 'scripts/profile.fish.in',
		output: 'libinput-config.fish',
		
		configuration: {
			'libdir': libdir,
			'project_name': project_name
		},
		
		install: true,
		install_dir: '/etc/fish/conf.d'
	)
	
	run_target('pre-uninstall', command: ['/bin/true'])
else
	meson.add_install_script('scripts/preload.sh', libdir, project_name)
	
	run_target('pre-uninstall',
		command: ['scripts/unpreload.sh', libdir, project_name]
	)
endif

shared_library('input-config',
	[
		'src/hook-gestures.c',
		'src/hook-init.c',
		'src/hook-pointer.c',
		'src/keyfile.c',
		'src/main.c',
		'src/override.c'
	],
	
	dependencies: [
		dependency('libinput').partial_dependency(includes: true),
		dependency('libudev').partial_dependency(includes: true),
		cc.find_library('dl', required: false)
	],
	
	link_args: '-Wl,--unresolved-symbols=ignore-all',
	
	install: true,
	install_dir: libdir
)
