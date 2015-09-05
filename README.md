gobot
=====

A machine learning algorithm for learning to score the final board in the game of Go

### Background

I wrote this for my machine learning final project during my senior of college.
[Here's the final paper I wrote.](https://github.com/burz/burz.github.io/blob/master/finalReport.pdf)
This paper reflects the state of gobot at the [finalProject](https://github.com/burz/gobot/tree/finalProject)
branch. It reads in `.sgf` files, and due to the linear nature of the `.sgf` file format, it must play through an
entire game to reach the final board position. Note that a stone can be placed where another had been placed before being
captured and removed from the board.

After I had submitted the project, I set out to up the learning algorithm by saving the boards to files rather than
running through the games each time. Furthermore, I reverted back to the
[original paper's algorithm](http://erikvanderwerf.tengen.nl/pubdown/learning_to_score_extended.pdf)
that I had based my project upon, and implemented my proposed bootstrapping method to generate good labels for
thousands of games (I was using datasets of 20,000+ games for my finalProject version of the algorithm).

### Current results

At the time, I was getting bad results, while using the bootstrap I had found that for many of the games in my dataset,
the reported score in the `.sgf` was incorrect. Usually it was off by 1 or 2 points from what the actual board should have
been scored in that state.

After further reasearch I discovered that most of these records assumed end game moves of varying complexity
would be played out, but the `.sgf`'s did not reflect these intuitions.
