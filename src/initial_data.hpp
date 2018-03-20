//
// Created by Julien Loiseau on 3/19/18.
//

#ifndef LANGFORD_GODFREY_INITIAL_DATA_HPP
#define LANGFORD_GODFREY_INITIAL_DATA_HPP

#include <cmath>
#include <string>

const size_t nb_colors = 4;
const size_t size_task = 6;

const size_t nb_cubes = 2*nb_colors;
const int64_t total_tasks = 1LL << size_task;
const size_t gray_shift = size_task + 3;

const size_t size_gray = 20;
const int64_t nb_gray = (1LL << size_gray) - 1;

#ifdef CUDA
    const size_t size_gray_gou = 20;
    const int64_t gray_gpu = (1LL << size_gray) + 1;
#endif

#endif //LANGFORD_GODFREY_INITIAL_DATA_HPP
