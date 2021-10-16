project('libinput-config', 'c')

cc = meson.get_compiler('c')

libdir = get_option('prefix') / get_option('libdir')
project_name = meson.project_name()

if get_option('shitty_sandboxing')
	libdir = '/etc'
endif

shared_library('input-config',
	[
		'src/hook.c',
		'src/keyfile.c',
		'src/main.c'
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
