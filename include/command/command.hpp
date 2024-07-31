#pragma once

#include <string>

typedef enum {
	single,
	broadcast
} Routing;

typedef struct {
	Routing route;
	std::string msg;
} CommandResponse;

class Command {
public:
  virtual ~Command() = default;

  virtual CommandResponse execute() const = 0;
};
