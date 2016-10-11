# Enso
Complete C++ source code for the Enso project. NuGet packages for Allegro library not included, you may use Visual Studio to restore them.

Enso is written on top of the DubuEngine code, which I've made for personal projects on free time. I dropped this project due to the realization that in order to finish it, I will definetly require funding. I lack graphics, art, music, etc. skills to make this into what I envisioned. But most importantly I simply don't have the time to take up on this as a hobby. I need to focus on other things, sadly.

DubuEngine is a basic 2D game engine I made using the Allegro library (and winsock for server/client). The code initially was NOT INTENDED to be published to the public. As a result you might notice how the style can be very messy and somewhat unreadable due to lack of commentary. I wrote the comments for myself, not for a stranger. Also many parts of the code are actually placeholders. I quite often find a better way to do something than I've done before, completely replacing my old code. Having that in mind I always focused on functionallity, and had the intention to tidy, clean up, rewrite, comment and finalize my code once I am 100% confident that it's the final version. I am well aware that this is horribly bad practice, and I will avoid doing this in the future.

Some of the main things I've learnt from working on this project:
  - Always comment the code so the reader knows what's up.
  - Try my best not to make 2 different classes dependent on each other. This overcomplicates things so much.
  - Class definitions should have their own separate headers from handlers for that class.
  - Just don't be lazy to create new files.
  - Create more functions for specific jobs. My nests are sometimes WAY too deep.
  
I have the option to tidy this up, but honestly it'll feel like a waste of time. I'm dropping this project to work on something simplier, smaller with more learning opportunities. Enso lost alot of learning potential as I've ended up working inside my own API.

My next projects will be much tidier, easier to read, more functional, less buggy. I'm keeping Enso as it is as a reminded to see how much I've improved in the future.
