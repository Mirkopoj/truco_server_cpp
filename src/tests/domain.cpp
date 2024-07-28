#include <criterion/criterion.h>
#include <criterion/internal/assert.h>

#include "../../include/truco_domain_engine.hpp"

Test(game, test) {
  TrucoBuilder gameBuilder;
  gameBuilder.add_player("A");
  gameBuilder.add_player("B");
  gameBuilder.hasta(15);
  Truco game = gameBuilder.build();
  cr_assert_none_throw(game.cantar_truco("A"));
  cr_assert_none_throw(game.cantar_re_truco("B"));
  cr_assert_none_throw(game.cantar_vale_cuatro("A"));
  cr_assert_none_throw(game.cantar_quiero("B"));
  cr_assert_none_throw(game.irse_al_maso("A"));
  cr_assert_none_throw(game.cantar_truco("B"));
  cr_assert_none_throw(game.cantar_re_truco("A"));
  cr_assert_none_throw(game.cantar_vale_cuatro("B"));
  cr_assert_none_throw(game.cantar_quiero("A"));
  cr_assert_none_throw(game.tirar_carta("B", 0));
  cr_assert_none_throw(game.irse_al_maso("A"));
  cr_assert_none_throw(game.cantar_truco("A"));
  cr_assert_none_throw(game.cantar_re_truco("B"));
  cr_assert_none_throw(game.cantar_vale_cuatro("A"));
  cr_assert_none_throw(game.cantar_quiero("B"));
  cr_assert_none_throw(game.irse_al_maso("A"));
  cr_assert_none_throw(game.cantar_truco("B"));
  cr_assert_none_throw(game.cantar_re_truco("A"));
  cr_assert_none_throw(game.cantar_vale_cuatro("B"));
  cr_assert_none_throw(game.cantar_quiero("A"));
  cr_assert_none_throw(game.tirar_carta("B", 0));
  cr_assert(!game.ganador().has_value());
  cr_assert_none_throw(game.irse_al_maso("A"));
  cr_assert_any_throw(game.irse_al_maso("B"));
  cr_assert_any_throw(game.irse_al_maso("A"));
  cr_assert(game.terminado());
  cr_assert(game.ganador().has_value());
  cr_assert_eq(game.ganador().value(), Equipo::Ellos);
}
