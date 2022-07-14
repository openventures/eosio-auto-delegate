#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

CONTRACT autodelegate : public eosio::contract {
public:
  using eosio::contract::contract;

  [[eosio::on_notify("eosio.token::transfer")]] void receive_token_transfer(
      eosio::name & from, eosio::name & to, eosio::asset & quantity,
      std::string & memo) {
    if (from == get_self() || to != get_self()) {
      return;
    }

    if (memo.rfind("stake:", 0) != 0) {
      return;
    }

    auto adu = autodelusers(get_self(), get_self().value);
    adu.require_find(from.value, "User is not on the allow list.");

    eosio::check(get_first_receiver() == eosio::name("eosio.token"),
                 "Invalid token contract");

    eosio::name target = eosio::name(memo.erase(0, 6));

    eosio::asset net_bw;
    net_bw.amount = 0;
    net_bw.symbol = quantity.symbol;

    eosio::asset cpu_bw;
    cpu_bw.amount = quantity.amount;
    cpu_bw.symbol = quantity.symbol;

    eosio::action(eosio::permission_level{get_self(), eosio::name("del")},
                  eosio::name("eosio"), eosio::name("delegatebw"),
                  std::make_tuple(get_self(), target, net_bw, cpu_bw, false))
        .send();
  }

  [[eosio::action]] void adduser(eosio::name user) {
    require_auth(get_self());
    auto adu = autodelusers(get_self(), get_self().value);
    auto it = adu.find(user.value);

    if (it != adu.end()) {
      return;
    }
    adu.emplace(get_self(), [&](auto &row) { row.user = user; });
  }

  [[eosio::action]] void rmuser(eosio::name user) {
    require_auth(get_self());

    auto adu = autodelusers(get_self(), get_self().value);
    auto e = adu.require_find(user.value, "User is not on the allow list.");

    adu.erase(e);
  }

private:
  struct [[eosio::table("autodelusers")]] autodeluser {
    eosio::name user;

    auto primary_key() const { return user.value; };
  };
  typedef eosio::multi_index<eosio::name("autodelusers"), autodeluser>
      autodelusers;
};
