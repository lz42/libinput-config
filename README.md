libinput-config allows you to configure your inputs in case your
Wayland compositor doesn't have a certain config or has none.

## How to build and install

```
meson build
cd build
meson compile
sudo meson install
```

If you're using GNOME Shell, you might be affected by [this Fedora
bug]. To work around it remove the capabilities from the shell's
executable file.

[this Fedora bug]: https://ask.fedoraproject.org/t/ld-library-path-is-not-set-by-bash-profile-anymore-after-upgrade-to-fedora-31/4247 "LD_LIBRARY_PATH is not set by .bash_profile anymore after upgrade to Fedora 31"

```
sudo setcap "" /usr/bin/gnome-shell
```

## How to use

The library reads the file `/etc/libinput.conf`, which consists of
`key=value` entries. The following settings are implemented:

```
tap {disabled,enabled}
tap-button-map {lrm,lmr}
drag {disabled,enabled}
drag-lock {disabled,enabled}
accel-speed [number]
accel-profile {none,flat,adaptive}
natural-scroll {disabled,enabled}
left-handed {disabled,enabled}
click-method {none,button-areas,clickfinger}
middle-emulation {disabled,enabled}
scroll-method {none,two-fingers,edge,on-button-down}
scroll-button [number]
dwt {disabled,enabled}
scroll-factor [number]
discrete-scroll-factor [number]
```

## How it works

The library wraps around libinput and hacks into the event loop to
read the config and configure devices. Additional hackery is used to
configure scrolling sensitivity. To do all of this, it uses the
`LD_PRELOAD` environment variable, which is modified by a
`/etc/profile.d` script.

## Inspiration

* [libinput-touchpad-scroll-fix](https://gitlab.com/kirbykevinson/libinput-touchpad-scroll-fix)
