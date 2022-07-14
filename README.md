# autodelegate EOSIO contract

Send the native `eosio.token` to the contract with the memo `stake:accountname` and
it will stake all incoming tokens to the `accountname` specificed in the memo as CPU.

It doesn't transfer the stake, so it can be used as a cold storage account with auto staking to
a hot wallet.

The account to which this contract is deployed needs the permission `del` with `eosio.code` delegation
and the linked auth: `eosio::delegatebw`. See [https://wax-test.bloks.io/account/stakehub1111](https://wax-test.bloks.io/account/stakehub1111) on the WAX test net for example usage.


The contract also manages an allow list to limit auto bandwidth delegation to a few trusted users.


If the memo doesn't start with `stake:` the contract is basically a noop and *won't* reject the transfer.
