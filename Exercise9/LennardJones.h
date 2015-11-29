/* File:   LennardJones.h */
/* Copyright 2015 ETH Zurich. All Rights Reserved. */
#ifndef LENNARD_JONES_H_AHQBPV5S
#define LENNARD_JONES_H_AHQBPV5S

#include <cassert>
#include "types.h"

/* TODO: Step 0: Include appropriate header to access AVX intrinsics */
#include <x86intrin.h>


class LennardJones
{
public:
    LennardJones(const value_type rm, const value_type epsilon)
    : rm_sq_(rm*rm), eps_(epsilon)
    { }


    inline value_type diff(configuration_type const& c, std::pair<value_type,value_type> const& newpos) const
    {
        assert( c.first.size() == c.second.size() );

        const std::size_t N = c.first.size()-1;

        value_type dE = 0.0;

        for (std::size_t i=0; i < N; ++i) {
            const value_type dx = c.first[N] - c.first[i];
            const value_type new_dx = newpos.first - c.first[i];
            const value_type dy = c.second[N] - c.second[i];
            const value_type new_dy = newpos.second - c.second[i];
            const value_type rm_d_sq = rm_sq_ / (dx*dx + dy*dy);
            const value_type new_rm_d_sq = rm_sq_ / (new_dx*new_dx + new_dy*new_dy);
            const value_type rm_d_6th = rm_d_sq*rm_d_sq*rm_d_sq;
            const value_type new_rm_d_6th = new_rm_d_sq*new_rm_d_sq*new_rm_d_sq;

            dE += eps_*((new_rm_d_6th*new_rm_d_6th - 2*new_rm_d_6th) - (rm_d_6th*rm_d_6th - 2*rm_d_6th));
        }

        return dE;
    }


    inline value_type diff_avx(configuration_type const& c, std::pair<value_type,value_type> const& newpos) const
    {
        assert( c.first.size() == c.second.size() );

        const std::size_t N = c.first.size()-1;



        // return dE;
        return 0.0;
    }


    inline value_type operator()(configuration_type const& c) const
    {
        assert( c.first.size() == c.second.size() );
        std::size_t const n = c.first.size();
        value_type energy = 0.0;
        for(std::size_t i=0; i < n; ++i) {
            value_type energy_part = 0.0;
            for(std::size_t j=0; j < i; ++j)
            {
                value_type const dx = c.first[i] - c.first[j];
                value_type const dy = c.second[i] - c.second[j];
                value_type const rm_d_sq = rm_sq_ / (dx*dx + dy*dy);
                value_type const rm_d_6th = rm_d_sq*rm_d_sq*rm_d_sq;
                energy_part += eps_*(rm_d_6th*rm_d_6th - 2*rm_d_6th);
            }
            energy += energy_part;
        }
        return energy;
    }


private:

    const value_type rm_sq_;
    const value_type eps_;
};

#endif /* LENNARD_JONES_H_AHQBPV5S */
