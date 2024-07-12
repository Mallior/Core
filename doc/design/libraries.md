# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libmallior_cli*         | RPC client functionality used by *mallior-cli* executable |
| *libmallior_common*      | Home for common functionality shared by different executables and libraries. Similar to *libmallior_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libmallior_consensus*   | Stable, backwards-compatible consensus functionality used by *libmallior_node* and *libmallior_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libmalliorconsensus*    | Shared library build of static *libmallior_consensus* library |
| *libmallior_kernel*      | Consensus engine and support library used for validation by *libmallior_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libmalliorqt*           | GUI functionality used by *mallior-qt* and *mallior-gui* executables |
| *libmallior_ipc*         | IPC functionality used by *mallior-node*, *mallior-wallet*, *mallior-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libmallior_node*        | P2P and RPC server functionality used by *malliord* and *mallior-qt* executables. |
| *libmallior_util*        | Home for common functionality shared by different executables and libraries. Similar to *libmallior_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libmallior_wallet*      | Wallet functionality used by *malliord* and *mallior-wallet* executables. |
| *libmallior_wallet_tool* | Lower-level wallet functionality used by *mallior-wallet* executable. |
| *libmallior_zmq*         | [ZeroMQ](../zmq.md) functionality used by *malliord* and *mallior-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libmallior_consensus* and *libmallior_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libmallior_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libmallior_node* code lives in `src/node/` in the `node::` namespace
  - *libmallior_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libmallior_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libmallior_util* code lives in `src/util/` in the `util::` namespace
  - *libmallior_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

mallior-cli[mallior-cli]-->libmallior_cli;

malliord[malliord]-->libmallior_node;
malliord[malliord]-->libmallior_wallet;

mallior-qt[mallior-qt]-->libmallior_node;
mallior-qt[mallior-qt]-->libmalliorqt;
mallior-qt[mallior-qt]-->libmallior_wallet;

mallior-wallet[mallior-wallet]-->libmallior_wallet;
mallior-wallet[mallior-wallet]-->libmallior_wallet_tool;

libmallior_cli-->libmallior_util;
libmallior_cli-->libmallior_common;

libmallior_common-->libmallior_consensus;
libmallior_common-->libmallior_util;

libmallior_kernel-->libmallior_consensus;
libmallior_kernel-->libmallior_util;

libmallior_node-->libmallior_consensus;
libmallior_node-->libmallior_kernel;
libmallior_node-->libmallior_common;
libmallior_node-->libmallior_util;

libmalliorqt-->libmallior_common;
libmalliorqt-->libmallior_util;

libmallior_wallet-->libmallior_common;
libmallior_wallet-->libmallior_util;

libmallior_wallet_tool-->libmallior_wallet;
libmallior_wallet_tool-->libmallior_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class mallior-qt,malliord,mallior-cli,mallior-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libmallior_wallet* and *libmallior_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libmallior_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libmallior_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libmallior_common* should serve a similar function as *libmallior_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libmallior_util* and *libmallior_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for mallior-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libmallior_kernel* is not supposed to depend on *libmallior_common*, only *libmallior_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libmallior_kernel* should only depend on *libmallior_util* and *libmallior_consensus*.

- The only thing that should depend on *libmallior_kernel* internally should be *libmallior_node*. GUI and wallet libraries *libmalliorqt* and *libmallior_wallet* in particular should not depend on *libmallior_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libmallior_consensus*, *libmallior_common*, and *libmallior_util*, instead of *libmallior_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libmalliorqt*, *libmallior_node*, *libmallior_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libmallior_node* to *libmallior_kernel* as part of [The libmalliorkernel Project #24303](https://github.com/mallior/mallior/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/mallior/mallior/issues/15732)
