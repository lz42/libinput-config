libinput-config allows you to configure your inputs in case your
Wayland compositor doesn't have a certain config or has none.

## How to build and install

```
meson build
cd build
meson compile
sudo meson install
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
speed [number]
```

## How it works

The library wraps around libinput and hacks into the event loop to
read the config and configure devices. Additional hackery is used to
configure scrolling sensitivity and pointer speed. To do all of this,
it uses the `/etc/ld.so.preload` file, which is modified by the
install script. If your libc doesn't support `/etc/ld.so.preload`, an
`LD_PRELOAD` fallback is used.

## Inspiration

* [libinput-touchpad-scroll-fix](https://gitlab.com/kirbykevinson/libinput-touchpad-scroll-fix)
