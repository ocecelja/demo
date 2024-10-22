Link Manager
============

The link manager project acts as an entry point into managing various types
of network links.

The project provides:

- A CLI, `linkmanager` - this can provide information on available links, but
  also be used to run as a daemon, in which case a management strategy is
  executed and metrics reported.
- A framework for implementing link modules. These must implement interfaces
  in `include/linkmanager/api/modules{.h|/*}` and can be built as standalone
  plugins. The `linkmanager` CLI can load such plugins.

Link Modules
------------

Link modules represent the logical links a hardware interface can provide.
This includes the hardware itself, which may be power cycled via this interface
(at least in principle). But it also means the Linux network devices the
hardware drivers eventually provide.

The distinction is necessary, as some hardware may provide more than one
network device. Such devices may simply be distinct logical interfaces, such as
an Ethernet and an IP device (similar to TUN vs. TAP devices), each of which
can be used in parallel to push data across the same cables or airwaves.

But an LTE modem may also, for example, provide support for multiple SIMs and
permit parallel communications across both channels.

For this reason, the module interface is split in three levels:

- `link_module` represents the physical hardware, and contains operations e.g.
  for power cycling.
- Each `link_module` contains one or more virtual `links`. Such links are
  configured separately. A `link_module` with two SIM slots, for example, can
  provide two distinct `links` if the SIM slots can be used for parallel
  communications. If only one SIM slot can be used at a time, a single `link`
  that needs to be reconfigured may be more sensible.
- Finally, each `link` can provide one or more `devices` - these correspond
  to Linux network devices.
