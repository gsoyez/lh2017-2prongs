# lh2017-2prongs

## setting things up

### Getting the framework

The framework is from the 2014PileupWorkshop github repository. This
is included as a (git) submodule of our lh2017-2prongs repository.

Details about git submodules can be found at

  https://git-scm.com/book/en/v2/Git-Tools-Submodules

but here's a basic digest:

 - Initial checkout:
   A default "git clone" will give you an empty 2014PileupWorkshop directory
   you then need to run
   
     git submodule init
     git submodule update

   An alternative is to pass the option "--recursive" to the orinal
   "git clone" command

 - Getting changes in the 2014PileupWorkshop submodule:

     git submodule update --remote 2014PileupWorkshop

### Building the framework

Detailed information on how to build and use the framework is
available from (some of it, including the fastjet-contrib part is
out-of-date)

  2014PileupWorkshop/README.md

In a nutshell, here's the plan

 - make sure you have a compiled version of fastjet
   see http://www.fastjet.fr

 - make sure you have a compiled version of fastjet-contribb
   see https://fastjet.hepforge.org/contrib/

 - there are compilation helpers in 2014PileupWorkshop/scripts
   (details in the next few steps)

 - whenever you want to build some code in a given directory, create a
   ".fastjet" file with the path to your fastjet installation (root
   path, without the /bin extension) in your current directory, then run

     ./mkmk
     make

 - to build the framework you should do that in the

     2014PileupWorkshop/Framework/

   If you want to have a look at (and build) the examples, you should
   do it in

     2014PileupWorkshop/examples/

   I (GS) can set up the mkmk scripts in other directories as we go along


