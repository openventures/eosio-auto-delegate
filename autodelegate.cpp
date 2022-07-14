#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

CONTRACT autodelegate : public eosio::contract
{
				public:
								using eosio::contract::contract;

								[[eosio::on_notify("eosio.token::transfer")]] void receive_token_transfer(
																eosio::name & from,
																eosio::name & to,
																eosio::asset & quantity,
																std::string & memo)
								{
												if (from == get_self() || to != get_self())
												{
																return;
												}


												if (memo.rfind("stake:", 0) != 0)
												{
																return;
												}

												eosio::check(get_first_receiver() == eosio::name("eosio.token"), "Invalid token contract");


												eosio::name target = eosio::name(memo.erase(0, 6));

												eosio::asset net_bw;
												net_bw.amount = 0;
												net_bw.symbol = quantity.symbol;

												eosio::asset cpu_bw;
												cpu_bw.amount = quantity.amount;
												cpu_bw.symbol = quantity.symbol;

												eosio::action(
																				eosio::permission_level{get_self(), eosio::name("del")},
																				eosio::name("eosio"),
																				eosio::name("delegatebw"),
																				std::make_tuple(
																								get_self(),
																								target,
																								net_bw,
																								cpu_bw,
																								false)
																		 ).send();
								}
};
