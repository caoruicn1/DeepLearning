/*----------------------------------------------------------------------------
 * Copyright (c) Microsoft Corp. 
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not 
 * use this file except in compliance with the License.  You may obtain a copy 
 * of the License at http://www.apache.org/licenses/LICENSE-2.0  
 *
 * THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY 
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED 
 * WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE, 
 * MERCHANTABLITY OR NON-INFRINGEMENT. 
 *
 * See the Apache Version 2.0 License for specific language governing 
 * permissions and limitations under the License.
 *
 *------------------------------------------------------------------------------------
 *
 * File: amp_rand_collection.h
 * 
 * This file defines the interface/methods necessary for a RNG collection
 *------------------------------------------------------------------------------------ */

#pragma once
#include <amp.h>

/// Implements collection base class
/// This class hides the container array
/// Defines necessary access kernel methods to get to RNG
template<class _rng_type, int _rank>
class amp_rand_collection
{
    typedef amp_rand_collection<_rng_type, _rank> _base;
private:
    amp_rand_collection()
    {
    }

protected:
    concurrency::array_view<_rng_type, _rank> m_rng_av;

public:
    amp_rand_collection(const concurrency::extent<_rank> rand_extent, int seed = 0) :
        m_rng_av(concurrency::array<_rng_type, _rank>(rand_extent))
    {

    }

    ~amp_rand_collection()
    {
        m_rng_av.discard_data();
    }

    _rng_type& operator[] (concurrency::index<_rank> idx) const restrict(amp)
    {
        return m_rng_av[idx];
    }

    _rng_type& operator() (int l0) const restrict(amp)
    {
        static_assert(_rank == 1, "rank must be 1");
        return m_rng_av(l0);
    }

    _rng_type& operator() (int l0, int l1) const restrict(amp)
    {
        static_assert(_rank == 2, "rank must be 2");
        return m_rng_av(l0, l1);
    }

    _rng_type& operator() (int l0, int l1, int l2) const restrict(amp)
    {
        static_assert(_rank == 3, "rank must be 3");
        return m_rng_av(l0, l1, l2);
    }

    concurrency::extent<_rank> extent() const restrict(amp, cpu)
    {
        return m_rng_av.extent;
    }
};
