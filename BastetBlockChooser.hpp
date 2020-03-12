/*
    Bastet - tetris clone with embedded bastard block chooser
    (c) 2005-2009 Federico Poloni <f.polonithirtyseven@sns.it> minus 37

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BASTET_BLOCK_CHOOSER_HPP
#define BASTET_BLOCK_CHOOSER_HPP

#include <array>
#include <boost/functional/hash.hpp>
#include <boost/unordered_set.hpp>

#include "BlockChooser.hpp"
#include "Well.hpp"

namespace Bastet {

    // bogus score assigned to combinations which cause game over
    static constexpr long GameOverScore = -1000;

    //  declared in Well.hpp
    // assigns a score to a position w + a number of extra
    // lines deleted while getting there
    long Evaluate(const Well * w, int extralines = 0);

    typedef BlockPosition Vertex;

    // generic visitor that "does something" with a possible drop position
    class WellVisitor {
       public:
        virtual ~WellVisitor() noexcept = default;

        virtual void Visit(BlockType b, const Well * well, Vertex v) = 0;
    };

    // for each block type, drops it (via a BestScoreVisitor) and sees which
    // block reaches the best score along the drop positions
    class RecursiveVisitor : public WellVisitor {
       public:
        RecursiveVisitor() { _scores.fill(GameOverScore); }
        virtual ~RecursiveVisitor() noexcept override = default;
        virtual void Visit(BlockType b, const Well * well, Vertex v);

        using ScoresList = std::array<long, 7>;
        const ScoresList & GetScores() const { return _scores; }

       private:
        ScoresList _scores;
    };

    // returns the max score over all drop positions
    class BestScoreVisitor : public WellVisitor {
       public:
        explicit BestScoreVisitor(int bonusLines = 0);
        virtual ~BestScoreVisitor() noexcept override = default;
        virtual void Visit(BlockType b, const Well * well, Vertex v);
        long         GetScore() const { return _score; }

       private:
        long _score;
        int  _bonusLines;
    };

    /**
     * Tries to drop a block in all possible positions, and invokes the visitor
     * on each one
     */
    class Searcher {
       public:
        Searcher(BlockType b, const Well * well, Vertex v,
                 WellVisitor * visitor);

       private:
        boost::unordered_set<Vertex> _visited;
        // std::set<Vertex> _visited; ^^ the above is more efficient, we need to
        // do many inserts
        BlockType     _block;
        const Well *  _well;
        WellVisitor * _visitor;
        void          DFSVisit(Vertex v);
    };

    class BastetBlockChooser : public BlockChooser {
       public:
        virtual ~BastetBlockChooser() noexcept = default;

        virtual Queue     GetStartingQueue();
        virtual BlockType GetNext(const Well * well, const Queue & q);
        /**
         * computes "scores" of the candidate next blocks by dropping them in
         * all possible positions and choosing the one that has the least
         * max_(drop positions) Evaluate(well)
         */
        std::array<long, 7> ComputeMainScores(const Well * well,
                                              BlockType    currentBlock);

       private:
    };

    // block chooser similar to the older bastet versions, does not give a block
    // preview
    class NoPreviewBlockChooser : public BlockChooser {
       public:
        virtual ~NoPreviewBlockChooser() noexcept = default;
        virtual Queue     GetStartingQueue();
        virtual BlockType GetNext(const Well * well, const Queue & q);
    };

}  // namespace Bastet

#endif  // BASTET_BLOCK_CHOOSER_HPP
