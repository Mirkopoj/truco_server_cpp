#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

extern "C" {

typedef enum {
  Nosotros = 0,
  Ellos = 1,
} Equipo;
}

class Truco {
public:
  Truco(Truco &&) = default;
  Truco(const Truco &) = delete;
  Truco &operator=(Truco &&) = default;
  Truco &operator=(const Truco &) = delete;
  ~Truco() = default;

  void irse_al_maso(const char *player);
  void cantar_quiero(const char *player);
  void cantar_no_quiero(const char *player);
  void cantar_envido(const char *player);
  void cantar_real_envido(const char *player);
  void cantar_falta_envido(const char *player);
  void cantar_truco(const char *player);
  void cantar_re_truco(const char *player);
  void cantar_vale_cuatro(const char *player);
  void tirar_carta(const char *player, size_t card);
  std::vector<std::string> valid_commands(const char *player);
  bool terminado();
  std::optional<Equipo> ganador();

  std::string print_state();
  std::string print_player(const char *player);

private:
  friend class TrucoBuilder;
  Truco(const void *truco_rust);
  const void *truco;
};

class TrucoBuilder {
public:
  TrucoBuilder();
  TrucoBuilder(TrucoBuilder &&);
  TrucoBuilder(const TrucoBuilder &);
  TrucoBuilder &operator=(TrucoBuilder &&);
  TrucoBuilder &operator=(const TrucoBuilder &);
  ~TrucoBuilder() = default;

  void add_player(const char *player);
  void hasta(uint8_t hasta);
  Truco build();

  static const uint8_t MAX_PLAYERS = 6;

private:
  const void *truco_builder;
  uint8_t count;
  bool tiene_limite;
};
