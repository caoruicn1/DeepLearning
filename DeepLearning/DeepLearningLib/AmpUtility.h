#pragma once

#include <array>
#include <tuple>

#include <amp.h>

namespace deep_learning_lib
{
    float atomic_fetch_add(float *_Dest, const float _Value) restrict(amp)
    {
        float oldVal = *_Dest;
        float newVal;
        do {
            newVal = oldVal + _Value;
        } while (!concurrency::atomic_compare_exchange(
            reinterpret_cast<unsigned int*>(_Dest),
            reinterpret_cast<unsigned int*>(&oldVal),
            (*(reinterpret_cast<unsigned int*>(&newVal)))));

        return newVal;
    }

    template<typename T, int Rank>
    void fill(concurrency::array<T, Rank>& arr, T initValue) 
    {
        concurrency::parallel_for_each(arr.extent,
            [&arr, initValue](concurrency::index<Rank> idx) restrict(amp)
        {
            arr[idx] = initValue;
        });
    }

    template<typename T, int Rank>
    void fill(concurrency::array_view<T, Rank>& arr, T initValue) 
    {
        concurrency::parallel_for_each(arr.extent,
            [=](concurrency::index<Rank> idx) restrict(amp)
        {
            arr[idx] = initValue;
        });
    }

    concurrency::extent<4> make_extent(int e0, int e1, int e2, int e3)
    {
        return concurrency::extent<4>(std::array<int, 4>{{e0, e1, e2, e3}}.data());
    }
}
