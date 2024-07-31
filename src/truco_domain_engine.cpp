#include "../include/truco_domain_engine.hpp"
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

extern "C" {

typedef enum {
  Ok = 0,
  Err = 1,
} ResultEnum;

typedef struct {
  ResultEnum tag;
  const char *error;
} CResult;

typedef enum {
  Some = 0,
  None = 1,
} OptionEnum;

typedef struct {
  OptionEnum tag;
  Equipo some;
} COptionEquipo;

typedef struct TrucoRust TrucoRust;

CResult irse_al_maso(const TrucoRust *ptr, const char *player);
CResult cantar_quiero(const TrucoRust *ptr, const char *player);
CResult cantar_no_quiero(const TrucoRust *ptr, const char *player);
CResult cantar_envido(const TrucoRust *ptr, const char *player);
CResult cantar_real_envido(const TrucoRust *ptr, const char *player);
CResult cantar_falta_envido(const TrucoRust *ptr, const char *player);
CResult cantar_truco(const TrucoRust *ptr, const char *player);
CResult cantar_re_truco(const TrucoRust *ptr, const char *player);
CResult cantar_vale_cuatro(const TrucoRust *ptr, const char *player);
CResult tirar_carta(const TrucoRust *ptr, const char *player, size_t card);

typedef struct {
  char **data;
  size_t length;
} CStringArray;

void free_string_array(CStringArray array);

CStringArray valid_commands(const TrucoRust *ptr, const char *player);
bool terminado(const TrucoRust *ptr);
COptionEquipo ganador(const TrucoRust *ptr);

typedef struct TrucoBuilderRust TrucoBuilderRust;

const TrucoBuilderRust *new_truco_builder();
void add_player(const TrucoBuilderRust *ptr, const char *player);
void hasta(const TrucoBuilderRust *ptr, uint8_t hasta);

typedef struct {
  ResultEnum tag;
  union {
    const TrucoRust *value;
    const TrucoBuilderRust *error;
  } content;
} CResultTruco;

CResultTruco build(const TrucoBuilderRust *ptr);
}

std::vector<std::string> convert_to_cpp_vector(CStringArray array) {
  std::vector<std::string> result;
  result.reserve(array.length);
  for (size_t i = 0; i < array.length; ++i) {
    result.push_back(std::string(array.data[i]));
  }
  free_string_array(array);
  return result;
}

TrucoBuilder::TrucoBuilder()
    : truco_builder((const void *)::new_truco_builder()), count(0),
      tiene_limite(false) {}

void TrucoBuilder::add_player(const char *player) {
  if (count >= MAX_PLAYERS) {
    throw std::runtime_error("Reached Max player limit");
  }
  ::add_player((const TrucoBuilderRust *)truco_builder, player);
  count++;
}

void TrucoBuilder::hasta(uint8_t hasta) {
  if (tiene_limite) {
    throw std::runtime_error("All ready was limited");
  }
  ::hasta((const TrucoBuilderRust *)truco_builder, hasta);
  tiene_limite = true;
}

Truco TrucoBuilder::build() {
  if (count >= MAX_PLAYERS || count % 2 != 0 || !tiene_limite) {
    throw std::runtime_error(
        "Can't build a game in this conditions => count: " +
        std::to_string(count) + ", limite: " + std::to_string(tiene_limite));
  }
  CResultTruco res = ::build((const TrucoBuilderRust *)truco_builder);
  switch (res.tag) {
  case Ok:
    return Truco((const void *)res.content.value);
  case Err:
    throw std::runtime_error("Building Failed");
  }
}

Truco::Truco(const void *truco_rust) : truco{truco_rust} {}

void Truco::irse_al_maso(const char *player) {
  CResult res = ::irse_al_maso((const TrucoRust *)truco, player);
  switch (res.tag) {
  case Ok:
    return;
  case Err:
    throw std::runtime_error(res.error);
  }
}

void Truco::cantar_quiero(const char *player) {
  CResult res = ::cantar_quiero((const TrucoRust *)truco, player);
  switch (res.tag) {
  case Ok:
    return;
  case Err:
    throw std::runtime_error(res.error);
  }
}

void Truco::cantar_no_quiero(const char *player) {
  CResult res = ::cantar_no_quiero((const TrucoRust *)truco, player);
  switch (res.tag) {
  case Ok:
    return;
  case Err:
    throw std::runtime_error(res.error);
  }
}

void Truco::cantar_envido(const char *player) {
  CResult res = ::cantar_envido((const TrucoRust *)truco, player);
  switch (res.tag) {
  case Ok:
    return;
  case Err:
    throw std::runtime_error(res.error);
  }
}

void Truco::cantar_real_envido(const char *player) {
  CResult res = ::cantar_real_envido((const TrucoRust *)truco, player);
  switch (res.tag) {
  case Ok:
    return;
  case Err:
    throw std::runtime_error(res.error);
  }
}

void Truco::cantar_falta_envido(const char *player) {
  CResult res = ::cantar_falta_envido((const TrucoRust *)truco, player);
  switch (res.tag) {
  case Ok:
    return;
  case Err:
    throw std::runtime_error(res.error);
  }
}

void Truco::cantar_truco(const char *player) {
  CResult res = ::cantar_truco((const TrucoRust *)truco, player);
  switch (res.tag) {
  case Ok:
    return;
  case Err:
    throw std::runtime_error(res.error);
  }
}

void Truco::cantar_re_truco(const char *player) {
  CResult res = ::cantar_re_truco((const TrucoRust *)truco, player);
  switch (res.tag) {
  case Ok:
    return;
  case Err:
    throw std::runtime_error(res.error);
  }
}

void Truco::cantar_vale_cuatro(const char *player) {
  CResult res = ::cantar_vale_cuatro((const TrucoRust *)truco, player);
  switch (res.tag) {
  case Ok:
    return;
  case Err:
    throw std::runtime_error(res.error);
  }
}

void Truco::tirar_carta(const char *player, size_t card) {
  CResult res = ::tirar_carta((const TrucoRust *)truco, player, card);
  switch (res.tag) {
  case Ok:
    return;
  case Err:
    throw std::runtime_error(res.error);
  }
}

std::vector<std::string> Truco::valid_commands(const char *player) {
  CStringArray res = ::valid_commands((const TrucoRust *)truco, player);
  return convert_to_cpp_vector(res);
}

bool Truco::terminado() { return ::terminado((const TrucoRust *)truco); }

std::optional<Equipo> Truco::ganador() {
  COptionEquipo res = ::ganador((const TrucoRust *)truco);
  switch (res.tag) {
  case Some:
    return res.some;
  case None:
    return {};
  }
}
