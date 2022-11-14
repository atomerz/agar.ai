#pragma once

#include <cstdint>

namespace agarai {

  constexpr uint16_t MAX_NUMBER_NEURONS = 20;

  constexpr uint16_t GENOME_INITIAL_LENGTH_MIN = 24;
  constexpr uint16_t GENOME_INITIAL_LENGTH_MAX = 24;

  constexpr uint16_t GENOME_MAX_LENGTH = 300;

  constexpr bool CHOOSE_PARENT_BY_FITNESS = true;
  constexpr bool SEXUAL_REPRODUCTION = true;

  constexpr double GENE_INSERTION_DELETION_RATE = 0;
  constexpr double DELETION_RATIO = 0.5;
  constexpr double POINT_MUTATION_RATE = 0.001;

}