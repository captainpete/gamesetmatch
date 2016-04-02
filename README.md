# gamesetmatch

.. is a very fast<sup>†</sup> C implementation
of [Microsoft's Trueskill™](http://research.microsoft.com/en-us/projects/trueskill/) algorithm.

The magic in this brew is courtesy of [dirtyfilty aka Caleb Anderson](https://github.com/dirtyfilthy).

The commandline utility accepts an input CSV file of matches.<br>
Output consists of match IDs with the player skill estimate (_μ_ and _σ_), _before_ each match.

- Currently supports games of 2 players
- Currently has no support for teams
- Currently has no support for ties
- Player keys must be 8 characters long
- Probably needs a good test suite

<sup>†</sup> ~160k updates in 1.2 seconds on a 3GHz Core i7.
