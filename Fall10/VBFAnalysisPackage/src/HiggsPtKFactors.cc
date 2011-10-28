#include "HiggsPtKFactors.h"



double HiggsPtKFactors(const float& pt, const float& mH)
{
  if(pt >= 0. && pt <  2.)
  {
    if(mH == 200.) return 1.40775; else if(mH == 250.) return 1.39634; else if(mH == 300.) return 1.28762; else if(mH == 350.) return 1.21004; else if(mH == 400.) return 1.1321; else if(mH == 450.) return 1.05861; else if(mH == 500.) return 1.11903; else if(mH == 550.) return 0.811961; else if(mH == 600.) return 1.28318; else return 1.; 
  }
  else if(pt >= 2. && pt <  3.)
  {
    if(mH == 200.) return 1.4013; else if(mH == 250.) return 1.37717; else if(mH == 300.) return 1.33088; else if(mH == 350.) return 1.31049; else if(mH == 400.) return 1.25241; else if(mH == 450.) return 1.20474; else if(mH == 500.) return 1.14494; else if(mH == 550.) return 1.08708; else if(mH == 600.) return 1.06747; else return 1.; 
  }
  else if(pt >= 3. && pt <  4.)
  {
    if(mH == 200.) return 1.35186; else if(mH == 250.) return 1.3136; else if(mH == 300.) return 1.25678; else if(mH == 350.) return 1.24745; else if(mH == 400.) return 1.2227; else if(mH == 450.) return 1.19838; else if(mH == 500.) return 1.16497; else if(mH == 550.) return 1.11497; else if(mH == 600.) return 1.05145; else return 1.; 
  }
  else if(pt >= 4. && pt <  5.)
  {
    if(mH == 200.) return 1.24496; else if(mH == 250.) return 1.23348; else if(mH == 300.) return 1.21852; else if(mH == 350.) return 1.19961; else if(mH == 400.) return 1.19037; else if(mH == 450.) return 1.16761; else if(mH == 500.) return 1.13408; else if(mH == 550.) return 1.07851; else if(mH == 600.) return 1.04327; else return 1.; 
  }
  else if(pt >= 5. && pt <  6.)
  {
    if(mH == 200.) return 1.23332; else if(mH == 250.) return 1.19973; else if(mH == 300.) return 1.17904; else if(mH == 350.) return 1.17366; else if(mH == 400.) return 1.16918; else if(mH == 450.) return 1.14761; else if(mH == 500.) return 1.09525; else if(mH == 550.) return 1.06893; else if(mH == 600.) return 1.0212; else return 1.; 
  }
  else if(pt >= 6. && pt <  7.)
  {
    if(mH == 200.) return 1.16603; else if(mH == 250.) return 1.14548; else if(mH == 300.) return 1.15745; else if(mH == 350.) return 1.14989; else if(mH == 400.) return 1.14061; else if(mH == 450.) return 1.07225; else if(mH == 500.) return 1.08863; else if(mH == 550.) return 1.04875; else if(mH == 600.) return 1.02266; else return 1.; 
  }
  else if(pt >= 7. && pt <  8.)
  {
    if(mH == 200.) return 1.11788; else if(mH == 250.) return 1.14008; else if(mH == 300.) return 1.12932; else if(mH == 350.) return 1.09924; else if(mH == 400.) return 1.11838; else if(mH == 450.) return 1.08037; else if(mH == 500.) return 1.04259; else if(mH == 550.) return 1.04777; else if(mH == 600.) return 1.03054; else return 1.; 
  }
  else if(pt >= 8. && pt <  9.)
  {
    if(mH == 200.) return 1.09779; else if(mH == 250.) return 1.1005; else if(mH == 300.) return 1.09388; else if(mH == 350.) return 1.08555; else if(mH == 400.) return 1.07523; else if(mH == 450.) return 1.0676; else if(mH == 500.) return 1.05574; else if(mH == 550.) return 1.02761; else if(mH == 600.) return 1.01391; else return 1.; 
  }
  else if(pt >= 9. && pt <  10.)
  {
    if(mH == 200.) return 1.08932; else if(mH == 250.) return 1.08126; else if(mH == 300.) return 1.08191; else if(mH == 350.) return 1.09437; else if(mH == 400.) return 1.04581; else if(mH == 450.) return 1.0496; else if(mH == 500.) return 1.01626; else if(mH == 550.) return 1.02208; else if(mH == 600.) return 0.997101; else return 1.; 
  }
  else if(pt >= 10. && pt <  11.)
  {
    if(mH == 200.) return 1.07421; else if(mH == 250.) return 1.071; else if(mH == 300.) return 1.0469; else if(mH == 350.) return 1.0696; else if(mH == 400.) return 1.04901; else if(mH == 450.) return 1.03557; else if(mH == 500.) return 1.02304; else if(mH == 550.) return 1.02154; else if(mH == 600.) return 0.999166; else return 1.; 
  }
  else if(pt >= 11. && pt <  12.)
  {
    if(mH == 200.) return 1.06886; else if(mH == 250.) return 1.06282; else if(mH == 300.) return 1.07493; else if(mH == 350.) return 1.05662; else if(mH == 400.) return 1.05191; else if(mH == 450.) return 1.04504; else if(mH == 500.) return 1.0265; else if(mH == 550.) return 1.01616; else if(mH == 600.) return 0.984873; else return 1.; 
  }
  else if(pt >= 12. && pt <  13.)
  {
    if(mH == 200.) return 1.07764; else if(mH == 250.) return 1.05793; else if(mH == 300.) return 1.05358; else if(mH == 350.) return 1.06152; else if(mH == 400.) return 1.02945; else if(mH == 450.) return 1.02709; else if(mH == 500.) return 1.02808; else if(mH == 550.) return 1.00765; else if(mH == 600.) return 0.993918; else return 1.; 
  }
  else if(pt >= 13. && pt <  14.)
  {
    if(mH == 200.) return 1.05717; else if(mH == 250.) return 1.04599; else if(mH == 300.) return 1.05494; else if(mH == 350.) return 1.05128; else if(mH == 400.) return 1.04236; else if(mH == 450.) return 1.03836; else if(mH == 500.) return 1.01727; else if(mH == 550.) return 1.02405; else if(mH == 600.) return 1.00358; else return 1.; 
  }
  else if(pt >= 14. && pt <  15.)
  {
    if(mH == 200.) return 1.03499; else if(mH == 250.) return 1.05066; else if(mH == 300.) return 1.0477; else if(mH == 350.) return 1.03855; else if(mH == 400.) return 1.02528; else if(mH == 450.) return 1.03608; else if(mH == 500.) return 1.02531; else if(mH == 550.) return 1.01859; else if(mH == 600.) return 0.989307; else return 1.; 
  }
  else if(pt >= 15. && pt <  16.)
  {
    if(mH == 200.) return 1.06702; else if(mH == 250.) return 1.04662; else if(mH == 300.) return 1.04398; else if(mH == 350.) return 1.03851; else if(mH == 400.) return 1.01354; else if(mH == 450.) return 1.03421; else if(mH == 500.) return 1.0254; else if(mH == 550.) return 1.01966; else if(mH == 600.) return 1.00891; else return 1.; 
  }
  else if(pt >= 16. && pt <  17.)
  {
    if(mH == 200.) return 1.05263; else if(mH == 250.) return 1.04958; else if(mH == 300.) return 1.04129; else if(mH == 350.) return 1.03888; else if(mH == 400.) return 1.03077; else if(mH == 450.) return 1.018; else if(mH == 500.) return 1.02461; else if(mH == 550.) return 1.02533; else if(mH == 600.) return 1.00677; else return 1.; 
  }
  else if(pt >= 17. && pt <  18.)
  {
    if(mH == 200.) return 1.04618; else if(mH == 250.) return 1.04882; else if(mH == 300.) return 1.02601; else if(mH == 350.) return 1.04155; else if(mH == 400.) return 1.02952; else if(mH == 450.) return 1.03171; else if(mH == 500.) return 1.01185; else if(mH == 550.) return 1.01738; else if(mH == 600.) return 1.00803; else return 1.; 
  }
  else if(pt >= 18. && pt <  19.)
  {
    if(mH == 200.) return 1.03582; else if(mH == 250.) return 1.04313; else if(mH == 300.) return 1.03194; else if(mH == 350.) return 1.06018; else if(mH == 400.) return 1.02454; else if(mH == 450.) return 1.02119; else if(mH == 500.) return 1.01952; else if(mH == 550.) return 1.00418; else if(mH == 600.) return 0.996789; else return 1.; 
  }
  else if(pt >= 19. && pt <  20.)
  {
    if(mH == 200.) return 1.03018; else if(mH == 250.) return 1.03047; else if(mH == 300.) return 1.04518; else if(mH == 350.) return 1.02846; else if(mH == 400.) return 1.03031; else if(mH == 450.) return 1.02259; else if(mH == 500.) return 1.03479; else if(mH == 550.) return 1.0287; else if(mH == 600.) return 1.04703; else return 1.; 
  }
  else if(pt >= 20. && pt <  21.)
  {
    if(mH == 200.) return 1.03446; else if(mH == 250.) return 1.02357; else if(mH == 300.) return 1.05912; else if(mH == 350.) return 1.02913; else if(mH == 400.) return 1.02233; else if(mH == 450.) return 1.03406; else if(mH == 500.) return 1.02129; else if(mH == 550.) return 1.02169; else if(mH == 600.) return 1.02003; else return 1.; 
  }
  else if(pt >= 21. && pt <  22.)
  {
    if(mH == 200.) return 1.06125; else if(mH == 250.) return 1.04001; else if(mH == 300.) return 1.01907; else if(mH == 350.) return 1.02903; else if(mH == 400.) return 1.02354; else if(mH == 450.) return 1.03177; else if(mH == 500.) return 1.02466; else if(mH == 550.) return 1.03526; else if(mH == 600.) return 1.01882; else return 1.; 
  }
  else if(pt >= 22. && pt <  23.)
  {
    if(mH == 200.) return 1.02316; else if(mH == 250.) return 1.03903; else if(mH == 300.) return 1.0596; else if(mH == 350.) return 1.02765; else if(mH == 400.) return 1.03928; else if(mH == 450.) return 1.03416; else if(mH == 500.) return 1.04353; else if(mH == 550.) return 1.04615; else if(mH == 600.) return 1.01184; else return 1.; 
  }
  else if(pt >= 23. && pt <  24.)
  {
    if(mH == 200.) return 1.02227; else if(mH == 250.) return 1.04279; else if(mH == 300.) return 1.0456; else if(mH == 350.) return 1.03455; else if(mH == 400.) return 1.01557; else if(mH == 450.) return 1.02463; else if(mH == 500.) return 1.0291; else if(mH == 550.) return 1.05334; else if(mH == 600.) return 1.01029; else return 1.; 
  }
  else if(pt >= 24. && pt <  25.)
  {
    if(mH == 200.) return 1.01306; else if(mH == 250.) return 1.0206; else if(mH == 300.) return 1.03593; else if(mH == 350.) return 1.03576; else if(mH == 400.) return 1.04664; else if(mH == 450.) return 1.03828; else if(mH == 500.) return 1.01863; else if(mH == 550.) return 1.0255; else if(mH == 600.) return 1.01853; else return 1.; 
  }
  else if(pt >= 25. && pt <  26.)
  {
    if(mH == 200.) return 1.02091; else if(mH == 250.) return 1.03229; else if(mH == 300.) return 1.03139; else if(mH == 350.) return 1.04482; else if(mH == 400.) return 1.04765; else if(mH == 450.) return 1.05262; else if(mH == 500.) return 1.03191; else if(mH == 550.) return 1.01401; else if(mH == 600.) return 1.01038; else return 1.; 
  }
  else if(pt >= 26. && pt <  27.)
  {
    if(mH == 200.) return 1.00579; else if(mH == 250.) return 1.00122; else if(mH == 300.) return 1.02534; else if(mH == 350.) return 1.05203; else if(mH == 400.) return 1.03556; else if(mH == 450.) return 1.04512; else if(mH == 500.) return 1.01211; else if(mH == 550.) return 1.02712; else if(mH == 600.) return 1.03144; else return 1.; 
  }
  else if(pt >= 27. && pt <  28.)
  {
    if(mH == 200.) return 1.02884; else if(mH == 250.) return 0.998524; else if(mH == 300.) return 1.01115; else if(mH == 350.) return 1.04453; else if(mH == 400.) return 1.03611; else if(mH == 450.) return 1.03644; else if(mH == 500.) return 1.03187; else if(mH == 550.) return 1.03172; else if(mH == 600.) return 1.02039; else return 1.; 
  }
  else if(pt >= 28. && pt <  29.)
  {
    if(mH == 200.) return 1.01707; else if(mH == 250.) return 1.02126; else if(mH == 300.) return 1.02814; else if(mH == 350.) return 1.04091; else if(mH == 400.) return 1.04137; else if(mH == 450.) return 1.05141; else if(mH == 500.) return 1.02901; else if(mH == 550.) return 1.03489; else if(mH == 600.) return 1.0128; else return 1.; 
  }
  else if(pt >= 29. && pt <  30.)
  {
    if(mH == 200.) return 1.01251; else if(mH == 250.) return 1.02526; else if(mH == 300.) return 1.00475; else if(mH == 350.) return 1.03424; else if(mH == 400.) return 1.02564; else if(mH == 450.) return 1.0316; else if(mH == 500.) return 1.03446; else if(mH == 550.) return 1.04499; else if(mH == 600.) return 1.01181; else return 1.; 
  }
  else if(pt >= 30. && pt <  31.)
  {
    if(mH == 200.) return 1.00424; else if(mH == 250.) return 1.02997; else if(mH == 300.) return 1.03251; else if(mH == 350.) return 1.01405; else if(mH == 400.) return 1.02893; else if(mH == 450.) return 1.01995; else if(mH == 500.) return 1.03674; else if(mH == 550.) return 1.04615; else if(mH == 600.) return 1.03048; else return 1.; 
  }
  else if(pt >= 31. && pt <  32.)
  {
    if(mH == 200.) return 1.0177; else if(mH == 250.) return 1.01664; else if(mH == 300.) return 1.0131; else if(mH == 350.) return 1.00436; else if(mH == 400.) return 1.03009; else if(mH == 450.) return 1.02587; else if(mH == 500.) return 1.03581; else if(mH == 550.) return 1.03468; else if(mH == 600.) return 1.02776; else return 1.; 
  }
  else if(pt >= 32. && pt <  33.)
  {
    if(mH == 200.) return 0.990235; else if(mH == 250.) return 1.01938; else if(mH == 300.) return 1.02345; else if(mH == 350.) return 1.01591; else if(mH == 400.) return 1.03232; else if(mH == 450.) return 1.03481; else if(mH == 500.) return 1.02951; else if(mH == 550.) return 1.02699; else if(mH == 600.) return 1.03337; else return 1.; 
  }
  else if(pt >= 33. && pt <  34.)
  {
    if(mH == 200.) return 0.997125; else if(mH == 250.) return 1.00911; else if(mH == 300.) return 1.00803; else if(mH == 350.) return 1.02251; else if(mH == 400.) return 1.03476; else if(mH == 450.) return 1.03526; else if(mH == 500.) return 1.04242; else if(mH == 550.) return 1.0556; else if(mH == 600.) return 1.03972; else return 1.; 
  }
  else if(pt >= 34. && pt <  35.)
  {
    if(mH == 200.) return 0.980667; else if(mH == 250.) return 1.00339; else if(mH == 300.) return 1.00941; else if(mH == 350.) return 1.03229; else if(mH == 400.) return 1.02608; else if(mH == 450.) return 1.03902; else if(mH == 500.) return 1.04378; else if(mH == 550.) return 1.04821; else if(mH == 600.) return 1.03005; else return 1.; 
  }
  else if(pt >= 35. && pt <  36.)
  {
    if(mH == 200.) return 0.973548; else if(mH == 250.) return 1.01045; else if(mH == 300.) return 0.989199; else if(mH == 350.) return 1.0225; else if(mH == 400.) return 1.03095; else if(mH == 450.) return 1.02838; else if(mH == 500.) return 1.02699; else if(mH == 550.) return 1.02752; else if(mH == 600.) return 1.04737; else return 1.; 
  }
  else if(pt >= 36. && pt <  37.)
  {
    if(mH == 200.) return 1.00688; else if(mH == 250.) return 0.998588; else if(mH == 300.) return 1.024; else if(mH == 350.) return 0.999978; else if(mH == 400.) return 1.03632; else if(mH == 450.) return 1.03733; else if(mH == 500.) return 1.04342; else if(mH == 550.) return 1.01899; else if(mH == 600.) return 1.0477; else return 1.; 
  }
  else if(pt >= 37. && pt <  38.)
  {
    if(mH == 200.) return 0.98651; else if(mH == 250.) return 0.983476; else if(mH == 300.) return 1.01432; else if(mH == 350.) return 1.02407; else if(mH == 400.) return 1.03507; else if(mH == 450.) return 1.04028; else if(mH == 500.) return 1.02372; else if(mH == 550.) return 1.05624; else if(mH == 600.) return 1.02662; else return 1.; 
  }
  else if(pt >= 38. && pt <  39.)
  {
    if(mH == 200.) return 0.997925; else if(mH == 250.) return 0.983665; else if(mH == 300.) return 0.991813; else if(mH == 350.) return 1.01022; else if(mH == 400.) return 1.02803; else if(mH == 450.) return 1.02649; else if(mH == 500.) return 1.03208; else if(mH == 550.) return 1.03338; else if(mH == 600.) return 1.03916; else return 1.; 
  }
  else if(pt >= 39. && pt <  40.)
  {
    if(mH == 200.) return 0.977006; else if(mH == 250.) return 0.993356; else if(mH == 300.) return 1.00035; else if(mH == 350.) return 1.04114; else if(mH == 400.) return 1.00784; else if(mH == 450.) return 1.03593; else if(mH == 500.) return 1.04361; else if(mH == 550.) return 1.0416; else if(mH == 600.) return 1.03337; else return 1.; 
  }
  else if(pt >= 40. && pt <  41.)
  {
    if(mH == 200.) return 0.94214; else if(mH == 250.) return 1.00254; else if(mH == 300.) return 1.02079; else if(mH == 350.) return 1.01034; else if(mH == 400.) return 1.01398; else if(mH == 450.) return 1.0121; else if(mH == 500.) return 1.03834; else if(mH == 550.) return 1.042; else if(mH == 600.) return 1.04104; else return 1.; 
  }
  else if(pt >= 41. && pt <  42.)
  {
    if(mH == 200.) return 0.954742; else if(mH == 250.) return 0.978132; else if(mH == 300.) return 1.02117; else if(mH == 350.) return 1.01514; else if(mH == 400.) return 1.00873; else if(mH == 450.) return 1.02537; else if(mH == 500.) return 1.01824; else if(mH == 550.) return 1.02295; else if(mH == 600.) return 1.03612; else return 1.; 
  }
  else if(pt >= 42. && pt <  43.)
  {
    if(mH == 200.) return 0.978627; else if(mH == 250.) return 0.980319; else if(mH == 300.) return 0.997495; else if(mH == 350.) return 1.00201; else if(mH == 400.) return 1.01143; else if(mH == 450.) return 1.02678; else if(mH == 500.) return 1.03976; else if(mH == 550.) return 1.05228; else if(mH == 600.) return 1.01905; else return 1.; 
  }
  else if(pt >= 43. && pt <  44.)
  {
    if(mH == 200.) return 0.951728; else if(mH == 250.) return 0.983179; else if(mH == 300.) return 0.990144; else if(mH == 350.) return 1.01346; else if(mH == 400.) return 1.01373; else if(mH == 450.) return 1.01213; else if(mH == 500.) return 1.03402; else if(mH == 550.) return 1.04607; else if(mH == 600.) return 1.04507; else return 1.; 
  }
  else if(pt >= 44. && pt <  45.)
  {
    if(mH == 200.) return 0.948842; else if(mH == 250.) return 0.988759; else if(mH == 300.) return 1.006; else if(mH == 350.) return 1.01939; else if(mH == 400.) return 1.00487; else if(mH == 450.) return 1.022; else if(mH == 500.) return 1.01595; else if(mH == 550.) return 1.0244; else if(mH == 600.) return 1.04507; else return 1.; 
  }
  else if(pt >= 45. && pt <  46.)
  {
    if(mH == 200.) return 0.919378; else if(mH == 250.) return 0.967728; else if(mH == 300.) return 0.980382; else if(mH == 350.) return 0.998385; else if(mH == 400.) return 1.00919; else if(mH == 450.) return 1.01895; else if(mH == 500.) return 1.03791; else if(mH == 550.) return 1.0424; else if(mH == 600.) return 1.04109; else return 1.; 
  }
  else if(pt >= 46. && pt <  47.)
  {
    if(mH == 200.) return 0.943017; else if(mH == 250.) return 0.957893; else if(mH == 300.) return 0.977988; else if(mH == 350.) return 0.98526; else if(mH == 400.) return 1.00516; else if(mH == 450.) return 1.01254; else if(mH == 500.) return 1.0276; else if(mH == 550.) return 0.997026; else if(mH == 600.) return 1.05382; else return 1.; 
  }
  else if(pt >= 47. && pt <  48.)
  {
    if(mH == 200.) return 0.91548; else if(mH == 250.) return 0.96587; else if(mH == 300.) return 0.981212; else if(mH == 350.) return 0.998056; else if(mH == 400.) return 1.02147; else if(mH == 450.) return 1.01158; else if(mH == 500.) return 1.03462; else if(mH == 550.) return 1.04496; else if(mH == 600.) return 1.04039; else return 1.; 
  }
  else if(pt >= 48. && pt <  49.)
  {
    if(mH == 200.) return 0.917809; else if(mH == 250.) return 0.960781; else if(mH == 300.) return 0.968629; else if(mH == 350.) return 0.99147; else if(mH == 400.) return 1.01824; else if(mH == 450.) return 1.02543; else if(mH == 500.) return 1.00968; else if(mH == 550.) return 1.02895; else if(mH == 600.) return 1.05416; else return 1.; 
  }
  else if(pt >= 49. && pt <  50.)
  {
    if(mH == 200.) return 0.920506; else if(mH == 250.) return 0.95447; else if(mH == 300.) return 0.977781; else if(mH == 350.) return 0.990965; else if(mH == 400.) return 1.00651; else if(mH == 450.) return 1.01585; else if(mH == 500.) return 1.02453; else if(mH == 550.) return 1.03346; else if(mH == 600.) return 1.04354; else return 1.; 
  }
  else if(pt >= 50. && pt <  52.)
  {
    if(mH == 200.) return 0.899472; else if(mH == 250.) return 0.935058; else if(mH == 300.) return 0.959989; else if(mH == 350.) return 0.97437; else if(mH == 400.) return 0.991266; else if(mH == 450.) return 1.00166; else if(mH == 500.) return 1.01121; else if(mH == 550.) return 1.02151; else if(mH == 600.) return 1.02868; else return 1.; 
  }
  else if(pt >= 52. && pt <  54.)
  {
    if(mH == 200.) return 0.890386; else if(mH == 250.) return 0.926798; else if(mH == 300.) return 0.952724; else if(mH == 350.) return 0.967742; else if(mH == 400.) return 0.985686; else if(mH == 450.) return 0.996767; else if(mH == 500.) return 1.0069; else if(mH == 550.) return 1.01851; else if(mH == 600.) return 1.02594; else return 1.; 
  }
  else if(pt >= 54. && pt <  56.)
  {
    if(mH == 200.) return 0.88143; else if(mH == 250.) return 0.918621; else if(mH == 300.) return 0.94547; else if(mH == 350.) return 0.961154; else if(mH == 400.) return 0.980166; else if(mH == 450.) return 0.991943; else if(mH == 500.) return 1.00269; else if(mH == 550.) return 1.01564; else if(mH == 600.) return 1.02327; else return 1.; 
  }
  else if(pt >= 56. && pt <  58.)
  {
    if(mH == 200.) return 0.872582; else if(mH == 250.) return 0.910542; else if(mH == 300.) return 0.93827; else if(mH == 350.) return 0.954651; else if(mH == 400.) return 0.97474; else if(mH == 450.) return 0.987225; else if(mH == 500.) return 0.998631; else if(mH == 550.) return 1.01291; else if(mH == 600.) return 1.02071; else return 1.; 
  }
  else if(pt >= 58. && pt <  60.)
  {
    if(mH == 200.) return 0.863821; else if(mH == 250.) return 0.902564; else if(mH == 300.) return 0.931147; else if(mH == 350.) return 0.948256; else if(mH == 400.) return 0.96943; else if(mH == 450.) return 0.982632; else if(mH == 500.) return 0.994724; else if(mH == 550.) return 1.01033; else if(mH == 600.) return 1.01829; else return 1.; 
  }
  else if(pt >= 60. && pt <  62.)
  {
    if(mH == 200.) return 0.855132; else if(mH == 250.) return 0.894685; else if(mH == 300.) return 0.924114; else if(mH == 350.) return 0.94198; else if(mH == 400.) return 0.964245; else if(mH == 450.) return 0.978173; else if(mH == 500.) return 0.990926; else if(mH == 550.) return 1.00789; else if(mH == 600.) return 1.01599; else return 1.; 
  }
  else if(pt >= 62. && pt <  64.)
  {
    if(mH == 200.) return 0.846496; else if(mH == 250.) return 0.886898; else if(mH == 300.) return 0.917174; else if(mH == 350.) return 0.935829; else if(mH == 400.) return 0.959182; else if(mH == 450.) return 0.973846; else if(mH == 500.) return 0.987373; else if(mH == 550.) return 1.00558; else if(mH == 600.) return 1.01383; else return 1.; 
  }
  else if(pt >= 64. && pt <  66.)
  {
    if(mH == 200.) return 0.837899; else if(mH == 250.) return 0.879196; else if(mH == 300.) return 0.910326; else if(mH == 350.) return 0.929798; else if(mH == 400.) return 0.954236; else if(mH == 450.) return 0.969645; else if(mH == 500.) return 0.983909; else if(mH == 550.) return 1.00338; else if(mH == 600.) return 1.01179; else return 1.; 
  }
  else if(pt >= 66. && pt <  68.)
  {
    if(mH == 200.) return 0.829329; else if(mH == 250.) return 0.871571; else if(mH == 300.) return 0.903566; else if(mH == 350.) return 0.923878; else if(mH == 400.) return 0.949398; else if(mH == 450.) return 0.965559; else if(mH == 500.) return 0.980566; else if(mH == 550.) return 1.00127; else if(mH == 600.) return 1.00986; else return 1.; 
  }
  else if(pt >= 68. && pt <  70.)
  {
    if(mH == 200.) return 0.820774; else if(mH == 250.) return 0.864011; else if(mH == 300.) return 0.896886; else if(mH == 350.) return 0.918061; else if(mH == 400.) return 0.944653; else if(mH == 450.) return 0.961577; else if(mH == 500.) return 0.97733; else if(mH == 550.) return 0.999234; else if(mH == 600.) return 1.00803; else return 1.; 
  }
  else if(pt >= 70. && pt <  73.)
  {
    if(mH == 200.) return 0.798713; else if(mH == 250.) return 0.843817; else if(mH == 300.) return 0.878266; else if(mH == 350.) return 0.900982; else if(mH == 400.) return 0.929184; else if(mH == 450.) return 0.947374; else if(mH == 500.) return 0.964333; else if(mH == 550.) return 0.987851; else if(mH == 600.) return 0.997198; else return 1.; 
  }
  else if(pt >= 73. && pt <  76.)
  {
    if(mH == 200.) return 0.78617; else if(mH == 250.) return 0.832911; else if(mH == 300.) return 0.868715; else if(mH == 350.) return 0.892758; else if(mH == 400.) return 0.922527; else if(mH == 450.) return 0.941834; else if(mH == 500.) return 0.959897; else if(mH == 550.) return 0.98506; else if(mH == 600.) return 0.994804; else return 1.; 
  }
  else if(pt >= 76. && pt <  79.)
  {
    if(mH == 200.) return 0.773587; else if(mH == 250.) return 0.822063; else if(mH == 300.) return 0.859262; else if(mH == 350.) return 0.884667; else if(mH == 400.) return 0.915978; else if(mH == 450.) return 0.936407; else if(mH == 500.) return 0.955569; else if(mH == 550.) return 0.9823; else if(mH == 600.) return 0.992495; else return 1.; 
  }
  else if(pt >= 79. && pt <  82.)
  {
    if(mH == 200.) return 0.760949; else if(mH == 250.) return 0.81125; else if(mH == 300.) return 0.849881; else if(mH == 350.) return 0.876675; else if(mH == 400.) return 0.909502; else if(mH == 450.) return 0.931054; else if(mH == 500.) return 0.951303; else if(mH == 550.) return 0.979529; else if(mH == 600.) return 0.990223; else return 1.; 
  }
  else if(pt >= 82. && pt <  85.)
  {
    if(mH == 200.) return 0.748248; else if(mH == 250.) return 0.80045; else if(mH == 300.) return 0.840544; else if(mH == 350.) return 0.868752; else if(mH == 400.) return 0.903064; else if(mH == 450.) return 0.925751; else if(mH == 500.) return 0.947061; else if(mH == 550.) return 0.976713; else if(mH == 600.) return 0.987954; else return 1.; 
  }
  else if(pt >= 85. && pt <  88.)
  {
    if(mH == 200.) return 0.735479; else if(mH == 250.) return 0.789644; else if(mH == 300.) return 0.831232; else if(mH == 350.) return 0.860868; else if(mH == 400.) return 0.896638; else if(mH == 450.) return 0.920456; else if(mH == 500.) return 0.94281; else if(mH == 550.) return 0.973818; else if(mH == 600.) return 0.985656; else return 1.; 
  }
  else if(pt >= 88. && pt <  91.)
  {
    if(mH == 200.) return 0.722638; else if(mH == 250.) return 0.778817; else if(mH == 300.) return 0.821924; else if(mH == 350.) return 0.852997; else if(mH == 400.) return 0.890197; else if(mH == 450.) return 0.915141; else if(mH == 500.) return 0.93852; else if(mH == 550.) return 0.970821; else if(mH == 600.) return 0.983302; else return 1.; 
  }
  else if(pt >= 91. && pt <  94.)
  {
    if(mH == 200.) return 0.709726; else if(mH == 250.) return 0.76796; else if(mH == 300.) return 0.812603; else if(mH == 350.) return 0.84512; else if(mH == 400.) return 0.883721; else if(mH == 450.) return 0.909782; else if(mH == 500.) return 0.934181; else if(mH == 550.) return 0.967699; else if(mH == 600.) return 0.980864; else return 1.; 
  }
  else if(pt >= 94. && pt <  97.)
  {
    if(mH == 200.) return 0.696746; else if(mH == 250.) return 0.757063; else if(mH == 300.) return 0.803256; else if(mH == 350.) return 0.83722; else if(mH == 400.) return 0.877195; else if(mH == 450.) return 0.904363; else if(mH == 500.) return 0.929762; else if(mH == 550.) return 0.964434; else if(mH == 600.) return 0.978321; else return 1.; 
  }
  else if(pt >= 97. && pt <  100.)
  {
    if(mH == 200.) return 0.683702; else if(mH == 250.) return 0.746117; else if(mH == 300.) return 0.793871; else if(mH == 350.) return 0.829281; else if(mH == 400.) return 0.870603; else if(mH == 450.) return 0.898867; else if(mH == 500.) return 0.925243; else if(mH == 550.) return 0.961018; else if(mH == 600.) return 0.975655; else return 1.; 
  }
  else if(pt >= 100. && pt <  105.)
  {
    if(mH == 200.) return 0.64902; else if(mH == 250.) return 0.714566; else if(mH == 300.) return 0.764757; else if(mH == 350.) return 0.802473; else if(mH == 400.) return 0.845668; else if(mH == 450.) return 0.875644; else if(mH == 500.) return 0.903504; else if(mH == 550.) return 0.940676; else if(mH == 600.) return 0.956634; else return 1.; 
  }
  else if(pt >= 105. && pt <  110.)
  {
    if(mH == 200.) return 0.627711; else if(mH == 250.) return 0.696646; else if(mH == 300.) return 0.74938; else if(mH == 350.) return 0.789428; else if(mH == 400.) return 0.834696; else if(mH == 450.) return 0.866418; else if(mH == 500.) return 0.895779; else if(mH == 550.) return 0.934551; else if(mH == 600.) return 0.951843; else return 1.; 
  }
  else if(pt >= 110. && pt <  115.)
  {
    if(mH == 200.) return 0.606246; else if(mH == 250.) return 0.678533; else if(mH == 300.) return 0.733798; else if(mH == 350.) return 0.776154; else if(mH == 400.) return 0.82342; else if(mH == 450.) return 0.856862; else if(mH == 500.) return 0.887671; else if(mH == 550.) return 0.927921; else if(mH == 600.) return 0.94661; else return 1.; 
  }
  else if(pt >= 115. && pt <  120.)
  {
    if(mH == 200.) return 0.584672; else if(mH == 250.) return 0.66023; else if(mH == 300.) return 0.717996; else if(mH == 350.) return 0.762628; else if(mH == 400.) return 0.811818; else if(mH == 450.) return 0.846952; else if(mH == 500.) return 0.87916; else if(mH == 550.) return 0.920775; else if(mH == 600.) return 0.940905; else return 1.; 
  }
  else if(pt >= 120. && pt <  125.)
  {
    if(mH == 200.) return 0.563017; else if(mH == 250.) return 0.641743; else if(mH == 300.) return 0.701969; else if(mH == 350.) return 0.748838; else if(mH == 400.) return 0.799881; else if(mH == 450.) return 0.836675; else if(mH == 500.) return 0.870235; else if(mH == 550.) return 0.913115; else if(mH == 600.) return 0.934722; else return 1.; 
  }
  else if(pt >= 125. && pt <  130.)
  {
    if(mH == 200.) return 0.547039; else if(mH == 250.) return 0.623082; else if(mH == 300.) return 0.685715; else if(mH == 350.) return 0.734777; else if(mH == 400.) return 0.787602; else if(mH == 450.) return 0.826024; else if(mH == 500.) return 0.86089; else if(mH == 550.) return 0.904945; else if(mH == 600.) return 0.928056; else return 1.; 
  }
  else if(pt >= 130. && pt <  135.)
  {
    if(mH == 200.) return 0.543849; else if(mH == 250.) return 0.604259; else if(mH == 300.) return 0.669237; else if(mH == 350.) return 0.720444; else if(mH == 400.) return 0.774981; else if(mH == 450.) return 0.814999; else if(mH == 500.) return 0.851127; else if(mH == 550.) return 0.896275; else if(mH == 600.) return 0.920909; else return 1.; 
  }
  else if(pt >= 135. && pt <  140.)
  {
    if(mH == 200.) return 0.540601; else if(mH == 250.) return 0.585313; else if(mH == 300.) return 0.652539; else if(mH == 350.) return 0.705838; else if(mH == 400.) return 0.762021; else if(mH == 450.) return 0.803603; else if(mH == 500.) return 0.840952; else if(mH == 550.) return 0.887118; else if(mH == 600.) return 0.913287; else return 1.; 
  }
  else if(pt >= 140. && pt <  145.)
  {
    if(mH == 200.) return 0.537351; else if(mH == 250.) return 0.566205; else if(mH == 300.) return 0.635697; else if(mH == 350.) return 0.690964; else if(mH == 400.) return 0.748727; else if(mH == 450.) return 0.791841; else if(mH == 500.) return 0.830372; else if(mH == 550.) return 0.877489; else if(mH == 600.) return 0.905201; else return 1.; 
  }
  else if(pt >= 145. && pt <  150.)
  {
    if(mH == 200.) return 0.534107; else if(mH == 250.) return 0.550666; else if(mH == 300.) return 0.618536; else if(mH == 350.) return 0.675863; else if(mH == 400.) return 0.735102; else if(mH == 450.) return 0.779719; else if(mH == 500.) return 0.819396; else if(mH == 550.) return 0.867404; else if(mH == 600.) return 0.896661; else return 1.; 
  }
  else if(pt >= 150. && pt <  155.)
  {
    if(mH == 200.) return 0.530878; else if(mH == 250.) return 0.548282; else if(mH == 300.) return 0.601235; else if(mH == 350.) return 0.660456; else if(mH == 400.) return 0.72119; else if(mH == 450.) return 0.767296; else if(mH == 500.) return 0.808014; else if(mH == 550.) return 0.85687; else if(mH == 600.) return 0.88767; else return 1.; 
  }
  else if(pt >= 155. && pt <  160.)
  {
    if(mH == 200.) return 0.527674; else if(mH == 250.) return 0.5459; else if(mH == 300.) return 0.583756; else if(mH == 350.) return 0.644825; else if(mH == 400.) return 0.706945; else if(mH == 450.) return 0.754455; else if(mH == 500.) return 0.796325; else if(mH == 550.) return 0.845954; else if(mH == 600.) return 0.87829; else return 1.; 
  }
  else if(pt >= 160. && pt <  165.)
  {
    if(mH == 200.) return 0.524505; else if(mH == 250.) return 0.54351; else if(mH == 300.) return 0.566108; else if(mH == 350.) return 0.62896; else if(mH == 400.) return 0.69241; else if(mH == 450.) return 0.741318; else if(mH == 500.) return 0.784236; else if(mH == 550.) return 0.834604; else if(mH == 600.) return 0.868465; else return 1.; 
  }
  else if(pt >= 165. && pt <  170.)
  {
    if(mH == 200.) return 0.521381; else if(mH == 250.) return 0.541124; else if(mH == 300.) return 0.555214; else if(mH == 350.) return 0.61287; else if(mH == 400.) return 0.677593; else if(mH == 450.) return 0.727867; else if(mH == 500.) return 0.771804; else if(mH == 550.) return 0.822872; else if(mH == 600.) return 0.858245; else return 1.; 
  }
  else if(pt >= 170. && pt <  175.)
  {
    if(mH == 200.) return 0.518314; else if(mH == 250.) return 0.538743; else if(mH == 300.) return 0.553162; else if(mH == 350.) return 0.596566; else if(mH == 400.) return 0.662504; else if(mH == 450.) return 0.714122; else if(mH == 500.) return 0.759041; else if(mH == 550.) return 0.810771; else if(mH == 600.) return 0.84764; else return 1.; 
  }
  else if(pt >= 175. && pt <  180.)
  {
    if(mH == 200.) return 0.515313; else if(mH == 250.) return 0.536369; else if(mH == 300.) return 0.551094; else if(mH == 350.) return 0.580056; else if(mH == 400.) return 0.647155; else if(mH == 450.) return 0.700064; else if(mH == 500.) return 0.745959; else if(mH == 550.) return 0.79832; else if(mH == 600.) return 0.836664; else return 1.; 
  }
  else if(pt >= 180. && pt <  185.)
  {
    if(mH == 200.) return 0.512391; else if(mH == 250.) return 0.534009; else if(mH == 300.) return 0.549009; else if(mH == 350.) return 0.56335; else if(mH == 400.) return 0.631557; else if(mH == 450.) return 0.685736; else if(mH == 500.) return 0.732575; else if(mH == 550.) return 0.785534; else if(mH == 600.) return 0.825332; else return 1.; 
  }
  else if(pt >= 185. && pt <  190.)
  {
    if(mH == 200.) return 0.509558; else if(mH == 250.) return 0.531668; else if(mH == 300.) return 0.546909; else if(mH == 350.) return 0.559911; else if(mH == 400.) return 0.615721; else if(mH == 450.) return 0.671137; else if(mH == 500.) return 0.718897; else if(mH == 550.) return 0.772431; else if(mH == 600.) return 0.813658; else return 1.; 
  }
  else if(pt >= 190. && pt <  195.)
  {
    if(mH == 200.) return 0.506823; else if(mH == 250.) return 0.529351; else if(mH == 300.) return 0.544797; else if(mH == 350.) return 0.558035; else if(mH == 400.) return 0.599658; else if(mH == 450.) return 0.65628; else if(mH == 500.) return 0.704941; else if(mH == 550.) return 0.759024; else if(mH == 600.) return 0.801656; else return 1.; 
  }
  else if(pt >= 195. && pt <  200.)
  {
    if(mH == 200.) return 0.504196; else if(mH == 250.) return 0.527066; else if(mH == 300.) return 0.542675; else if(mH == 350.) return 0.556127; else if(mH == 400.) return 0.583379; else if(mH == 450.) return 0.641175; else if(mH == 500.) return 0.690717; else if(mH == 550.) return 0.745329; else if(mH == 600.) return 0.789337; else return 1.; 
  }
  else if(pt >= 200. && pt <  205.)
  {
    if(mH == 200.) return 0.501685; else if(mH == 250.) return 0.524819; else if(mH == 300.) return 0.540547; else if(mH == 350.) return 0.554189; else if(mH == 400.) return 0.574563; else if(mH == 450.) return 0.625833; else if(mH == 500.) return 0.676238; else if(mH == 550.) return 0.73136; else if(mH == 600.) return 0.776715; else return 1.; 
  }
  else if(pt >= 205. && pt <  210.)
  {
    if(mH == 200.) return 0.4993; else if(mH == 250.) return 0.522619; else if(mH == 300.) return 0.538416; else if(mH == 350.) return 0.552223; else if(mH == 400.) return 0.57263; else if(mH == 450.) return 0.610263; else if(mH == 500.) return 0.661514; else if(mH == 550.) return 0.717131; else if(mH == 600.) return 0.763801; else return 1.; 
  }
  else if(pt >= 210. && pt <  215.)
  {
    if(mH == 200.) return 0.497051; else if(mH == 250.) return 0.520464; else if(mH == 300.) return 0.536288; else if(mH == 350.) return 0.550231; else if(mH == 400.) return 0.570653; else if(mH == 450.) return 0.594474; else if(mH == 500.) return 0.646556; else if(mH == 550.) return 0.702653; else if(mH == 600.) return 0.750608; else return 1.; 
  }
  else if(pt >= 215. && pt <  220.)
  {
    if(mH == 200.) return 0.494944; else if(mH == 250.) return 0.518369; else if(mH == 300.) return 0.534167; else if(mH == 350.) return 0.548215; else if(mH == 400.) return 0.568634; else if(mH == 450.) return 0.578476; else if(mH == 500.) return 0.631373; else if(mH == 550.) return 0.687939; else if(mH == 600.) return 0.737147; else return 1.; 
  }
  else if(pt >= 220. && pt <  225.)
  {
    if(mH == 200.) return 0.492987; else if(mH == 250.) return 0.516336; else if(mH == 300.) return 0.532057; else if(mH == 350.) return 0.54618; else if(mH == 400.) return 0.566575; else if(mH == 450.) return 0.562277; else if(mH == 500.) return 0.615961; else if(mH == 550.) return 0.672999; else if(mH == 600.) return 0.723426; else return 1.; 
  }
  else if(pt >= 225. && pt <  230.)
  {
    if(mH == 200.) return 0.491188; else if(mH == 250.) return 0.514373; else if(mH == 300.) return 0.529964; else if(mH == 350.) return 0.544126; else if(mH == 400.) return 0.564479; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.600373; else if(mH == 550.) return 0.657846; else if(mH == 600.) return 0.709457; else return 1.; 
  }
  else if(pt >= 230. && pt <  235.)
  {
    if(mH == 200.) return 0.489553; else if(mH == 250.) return 0.512484; else if(mH == 300.) return 0.527892; else if(mH == 350.) return 0.54206; else if(mH == 400.) return 0.562347; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.584574; else if(mH == 550.) return 0.642488; else if(mH == 600.) return 0.695249; else return 1.; 
  }
  else if(pt >= 235. && pt <  240.)
  {
    if(mH == 200.) return 0.488089; else if(mH == 250.) return 0.510674; else if(mH == 300.) return 0.525846; else if(mH == 350.) return 0.539986; else if(mH == 400.) return 0.560183; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.568587; else if(mH == 550.) return 0.626937; else if(mH == 600.) return 0.68081; else return 1.; 
  }
  else if(pt >= 240. && pt <  245.)
  {
    if(mH == 200.) return 0.486803; else if(mH == 250.) return 0.50895; else if(mH == 300.) return 0.52383; else if(mH == 350.) return 0.537906; else if(mH == 400.) return 0.55799; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.55242; else if(mH == 550.) return 0.611201; else if(mH == 600.) return 0.66615; else return 1.; 
  }
  else if(pt >= 245. && pt <  250.)
  {
    if(mH == 200.) return 0.485702; else if(mH == 250.) return 0.507316; else if(mH == 300.) return 0.521849; else if(mH == 350.) return 0.535825; else if(mH == 400.) return 0.555773; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.536079; else if(mH == 550.) return 0.595288; else if(mH == 600.) return 0.651275; else return 1.; 
  }
  else if(pt >= 250. && pt <  255.)
  {
    if(mH == 200.) return 0.484793; else if(mH == 250.) return 0.505777; else if(mH == 300.) return 0.519907; else if(mH == 350.) return 0.533745; else if(mH == 400.) return 0.553535; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.579207; else if(mH == 600.) return 0.636195; else return 1.; 
  }
  else if(pt >= 255. && pt <  260.)
  {
    if(mH == 200.) return 0.484079; else if(mH == 250.) return 0.504338; else if(mH == 300.) return 0.518009; else if(mH == 350.) return 0.531672; else if(mH == 400.) return 0.551278; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.562963; else if(mH == 600.) return 0.620915; else return 1.; 
  }
  else if(pt >= 260. && pt <  265.)
  {
    if(mH == 200.) return 0.483571; else if(mH == 250.) return 0.503003; else if(mH == 300.) return 0.51616; else if(mH == 350.) return 0.52961; else if(mH == 400.) return 0.549008; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.546563; else if(mH == 600.) return 0.605442; else return 1.; 
  }
  else if(pt >= 265. && pt <  270.)
  {
    if(mH == 200.) return 0.483272; else if(mH == 250.) return 0.501776; else if(mH == 300.) return 0.514363; else if(mH == 350.) return 0.527563; else if(mH == 400.) return 0.546726; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.530013; else if(mH == 600.) return 0.58978; else return 1.; 
  }
  else if(pt >= 270. && pt <  275.)
  {
    if(mH == 200.) return 0.483189; else if(mH == 250.) return 0.50066; else if(mH == 300.) return 0.512624; else if(mH == 350.) return 0.525535; else if(mH == 400.) return 0.544439; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.513318; else if(mH == 600.) return 0.573936; else return 1.; 
  }
  else if(pt >= 275. && pt <  280.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.499659; else if(mH == 300.) return 0.510945; else if(mH == 350.) return 0.52353; else if(mH == 400.) return 0.54215; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.557913; else return 1.; 
  }
  else if(pt >= 280. && pt <  285.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.498778; else if(mH == 300.) return 0.50933; else if(mH == 350.) return 0.521552; else if(mH == 400.) return 0.539862; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.541718; else return 1.; 
  }
  else if(pt >= 285. && pt <  290.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.498018; else if(mH == 300.) return 0.507784; else if(mH == 350.) return 0.519605; else if(mH == 400.) return 0.53758; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.525351; else return 1.; 
  }
  else if(pt >= 290. && pt <  295.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.497383; else if(mH == 300.) return 0.50631; else if(mH == 350.) return 0.517691; else if(mH == 400.) return 0.535308; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.508819; else return 1.; 
  }
  else if(pt >= 295. && pt <  300.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.496873; else if(mH == 300.) return 0.504909; else if(mH == 350.) return 0.515815; else if(mH == 400.) return 0.533049; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.492121; else return 1.; 
  }
  else if(pt >= 300. && pt <  310.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.48147; else if(mH == 300.) return 0.503585; else if(mH == 350.) return 0.51398; else if(mH == 400.) return 0.530809; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 310. && pt <  320.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481432; else if(mH == 300.) return 0.502345; else if(mH == 350.) return 0.512191; else if(mH == 400.) return 0.52859; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 320. && pt <  330.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.501187; else if(mH == 350.) return 0.510449; else if(mH == 400.) return 0.526394; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 330. && pt <  340.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.500114; else if(mH == 350.) return 0.508758; else if(mH == 400.) return 0.524229; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 340. && pt <  350.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.499129; else if(mH == 350.) return 0.507121; else if(mH == 400.) return 0.522096; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 350. && pt <  360.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.498234; else if(mH == 350.) return 0.505542; else if(mH == 400.) return 0.519998; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 360. && pt <  370.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.49743; else if(mH == 350.) return 0.504023; else if(mH == 400.) return 0.517938; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 370. && pt <  380.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.496722; else if(mH == 350.) return 0.502567; else if(mH == 400.) return 0.515921; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 380. && pt <  390.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.496108; else if(mH == 350.) return 0.501175; else if(mH == 400.) return 0.51395; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 390. && pt <  400.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.495592; else if(mH == 350.) return 0.499851; else if(mH == 400.) return 0.512029; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 400. && pt <  410.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.495173; else if(mH == 350.) return 0.498598; else if(mH == 400.) return 0.51016; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 410. && pt <  420.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.494852; else if(mH == 350.) return 0.497418; else if(mH == 400.) return 0.508347; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 420. && pt <  430.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.49463; else if(mH == 350.) return 0.496312; else if(mH == 400.) return 0.506593; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 430. && pt <  440.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.494507; else if(mH == 350.) return 0.495284; else if(mH == 400.) return 0.504901; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 440. && pt <  450.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.494483; else if(mH == 350.) return 0.494335; else if(mH == 400.) return 0.503275; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 450. && pt <  460.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.494557; else if(mH == 350.) return 0.493467; else if(mH == 400.) return 0.501718; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 460. && pt <  470.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.494557; else if(mH == 350.) return 0.492682; else if(mH == 400.) return 0.500233; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 470. && pt <  480.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.494557; else if(mH == 350.) return 0.491982; else if(mH == 400.) return 0.498823; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 480. && pt <  490.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.494557; else if(mH == 350.) return 0.491368; else if(mH == 400.) return 0.497491; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else if(pt >= 490. && pt <  500.)
  {
    if(mH == 200.) return 0.483326; else if(mH == 250.) return 0.481904; else if(mH == 300.) return 0.494557; else if(mH == 350.) return 0.490841; else if(mH == 400.) return 0.49624; else if(mH == 450.) return 0.579273; else if(mH == 500.) return 0.57982; else if(mH == 550.) return 0.586099; else if(mH == 600.) return 0.597757; else return 1.; 
  }
  else return 1.
}
