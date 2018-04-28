/** Tag family with 23 distinct codes.
    bits: 36,  minimum hamming: 15,  minimum complexity: 10

    Max bits corrected       False positive rate
            0                  0.00000003 %
            1                  0.00000124 %
            2                  0.00002232 %
            3                  0.00026130 %
            4                  0.00223281 %
            5                  0.01485051 %
            6                  0.08004195 %
            7                  0.35943383 %

    Generation time: 2474.843000 s

    Hamming distance between pairs of codes (accounting for rotation):

       0  0
       1  0
       2  0
       3  0
       4  0
       5  0
       6  0
       7  0
       8  0
       9  0
      10  0
      11  0
      12  0
      13  0
      14  0
      15  98
      16  106
      17  27
      18  13
      19  7
      20  2
      21  0
      22  0
      23  0
      24  0
      25  0
      26  0
      27  0
      28  0
      29  0
      30  0
      31  0
      32  0
      33  0
      34  0
      35  0
      36  0
**/
#pragma once
namespace AprilTags {

const unsigned long long t36h15[] = 
     { 0x23aeebda3L, 0x48484f755L, 0x5a950142eL, 0xa01ed81cdL, 0xaec2998e1L, 0xdaaddde1dL, 0x4c54b24d6L, 0x24441793dL, 0xe35bcf4b9L, 0x6e1ba8fa5L, 0x410ddc6eaL, 0xaa18716e6L, 0x72748f7e2L, 0x18a8d6ce7L, 0xdc7ec448bL, 0x027389b3eL, 0x8be5aef68L, 0xe6032f64cL, 0x262c36681L, 0x1702dcd09L, 0x859dbac3bL, 0xfada5605eL, 0x9fb02b4c9L };
    
static const TagCodes tagCodes36h15 = TagCodes(36, 15, t36h15, sizeof(t36h15)/sizeof(t36h15[0]));

}
