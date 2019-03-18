# "Genome-Matcher" DNA Sequence Search System

## Intro
This Software is aimed at helping researchers better search through and compare the DNA of bacteria responsible for common illnesses. The thought is that if the CDCs researchers have better tools to process the DNA of various organisms, they can more quickly identify new bacterial strains and create cures.

### Notice
Before running the search system, you need to change PROVIDED_DIR in the following line 15 of main.cpp into the absolute path of your"data" repository :
```
const string PROVIDED_DIR = "/Users/jin/Gee-nomics/data";
```

To get the absolute path of your "data" repository, you can perform the following steps:
On Mac OS X, you open up your terminal, change your directory into where you cloned your Genome-Matcher repository

```
$ cd data

$ pwd

/Users/jin/Genome-Matcher/data
```
Then replace PROVIDED_DIR with this path.


## Example
When you run the program, it might look like this:
```
28
Welcome to the Gee-nomics test harness!
The genome library is initially empty, with
Commands:
    c - create new genome library
    a - add one genome manually
    l - load one data file
    d - load all provided data files e - find matches exactly
    a default minSearchLength of 10
    s-find matching SNiPs
    r-find related genomes (manual) f-find related genomes (file) ?-show this menu
    q-quit
Enter command: a
Enter name: yeti
Enter DNA sequence: ACGTACGTAAAACCCCGGGGTTTTNANANANANA Enter command: e
Enter DNA sequence for which to find
Enter minimum sequence match length:
    1 matches of AAAACCCCGGGGTTNN found:
exact matches: AAAACCCCGGGGTTNN 12
    length 14 position 8 in yeti
Enter command: e
Enter DNA sequence for which to find
Enter minimum sequence match length:
    No matches of CCCCAAAATTTT were found.
Enter command: s
Enter DNA sequence for which to find exact matches and SNiPs: AAAACCTCGGGGTTNN Enter minimum sequence match length: 12
    1 matches and/or SniPs of AAAACCTCGGGGTTNN found:
    length 14 position 8 in yeti
Enter command: c
Enter minimum search length (3-100): 4
Enter command: a
Enter name: sasquatch
Enter DNA sequence: GGGGTTTTAAAACCCCACGTACGTACGTNANANANA Enter command: r
Enter DNA sequence: AAATCCCTGGGGTTTTNANA
Enter match percentage threshold (0-100): 20
Require (e)xact match or allow (S)NiPs (e or s): s
    1 related genomes were found: 50.00% sasquatch
Enter command: c
Enter minimum search length (3-100): 10
Enter command: d
    Loaded 1 genomes from Ferroplasma_acidarmanus.txt
    Loaded 2 genomes from Halobacterium_jilantaiense.txt
    Loaded 105 genomes from Halorubrum_chaoviator.txt
    Loaded 83 genomes from Halorubrum_californiense.txt
    Loaded 55 genomes from Halorientalis_regularis.txt
    Loaded 121 genomes from Halorientalis_persicus.txt
    Loaded 1 genomes from Ferroglobus_placidus.txt
    Loaded 1 genomes from Desulfurococcus_mucosus.txt
Enter command: e
Enter DNA sequence for which to find exact matches: ACGAATCACGTGCGAGA Enter minimum sequence match length: 11
    2 matches of ACGAATCACGTGCGAGA found:
    length 17 position 568 in NZ_AOJK01000080.1 Halorubrum californiensis DSM 19288 contig_80, whole genome shotgun sequence
    length 12 position 1977 in NZ_FOCX01000065.1 Halorientalis persicus strain IBRC- M 10043, whole genome shotgun sequence
Enter command: q
```
