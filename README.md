# PVM_Project
 
# الحمد للّه الذي بفضله تم القيام بهذا العمل نحمده و نستعين به و اسأل اللّه أن ينفعنا وينفعكم به

A program to multiply a square matrix and a vector of the same size.

HOW TO RUN THIS PROJECT?

"./pvmCompare"

you have to install and prepare pvm before first attempt to run.


HOW TO RUN ANY PVM PROGRAMS:

1-Download and Install pvm package for your platform from http://www.netlib.org/pvm3/

2-copy all files plus "required_files" directory contents to this path /home/user_name/bin/pvm3/bin/your_pc_architecture (example: /home/louay/bin/pvm3/bin/LINUX64)(Note:I'm using LINXU64 so if you use something different you have to copy similar files "found in required_files" from where you installed your pvm package).

3-run pvmd3 in a separate terminal/cmd window.

4-compile "cc your_pvm_program.c -o your_pvm_program *.a" // *.a to link to all libraries required

5-run "./your_pvm_program"

NEED A REFERENCE TO LEARN?

I recommend this book(http://www.netlib.org/pvm3/book/node1.html) yet I mostly advice reading two chapters:

1-PVM User Interface

2-Program Examples




![](https://github.com/aloshlouay/PVM_Project/blob/master/pictures/0.png)

                                                   PVM Single Row
<div dir="rtl">
1-تقوم المهمة اﻷب بقراءة بيانات مصفوفة الدخل وشعاع الدخل من الملفات              (readArray.txt, readVector.txt) باستخدام التابع ReadData الموجود في الملف supportingArrays.h

2-ينشئ اﻷب عدد من المهمات (اﻷولاد) مساوٍ لإحدى بُعدي المصفوفة (علما أن المصفوفة يجب أن تكون مربعة)

3-الإرسال:
يرسل اﻷب سطر من المصفوفة مع شعاع الدخل المضروب فيه لكل ابن على التوالي
بحيث يرسل للابن الأول السطر الأول من المصفوفة مع شعاع الدخل و للابن الثاني السطر الثاني من المصفوفة مع شعاع الدخل و هكذا

![](https://github.com/aloshlouay/PVM_Project/blob/master/pictures/pvmsinglerow.png)

4-إستقبال الابن:
يستقبل الابن شعاعين من الأب (سطر من المصوفة الموافقة لترتيبه مع شعاع الدخل) ويضرب الابن الشعاعين باستخدام التابع vectorXvector فيحصل على نتيجة (رقم وحيد) يقوم بإرساله إلى المهمة الأب

5-يستقبل الأب(رقم وحيد) عدد من المرات مساوٍ لعدد ابنائه ولكنه لا يشترط ترتيب الاستقبال فمن فرغ من الابناء من حساباته يرسل للاب واﻷب بستقبل من دون ترتيب.
يرتب اﻷب النتائج التي وصلت إليه باستخدام متغير مُنشئ index
حيث index = tid – children_ids[0] 

tid رقم التعريف الخاص للمهة التي ارسلت هذه الرسالة "المستقبلة عند الأب".
بعد حساب index يمكن وضع النتيجة المستقبلة في شعاع الخرج
result_vector[index] = result_value


                                                   PVM Single COl

1-تقوم المهمة اﻷب بقراءة بيانات مصفوفة الدخل وشعاع الدخل من الملفات             (readArray.txt, readVector.txt) باستخدام التابع ReadData الموجود في الملف supportingArrays.h

2-ينشئ اﻷب عدد من المهمات (اﻷولاد) مساوٍ لإحدى بُعدي المصفوفة (علما أن المصفوفة يجب أن تكون مربعة)

3-الارسال:
يرسل الأب لكل مهمة ابن عامود من مصفوفة الدخل و عنصر واحد فقط من شعاع الدخل
 بحيث يرسل للابن الأول اول عامود مع أول عنصر من شعاع الدخل و للابن الثاني ثاني عامود مع العنصر الثاني من شعاع الدخل و هكذا

![](https://github.com/aloshlouay/PVM_Project/blob/master/pictures/pvmsinglecol.png)

4-إستقبال الابن:
يستقبل الابن شعاع(يمثل العامود المختار من الاب وفقاً لترتيب المهمة الابن) مع قيمة وحيدة ويقوم بعملية الضرب باستخدام التابعvectorScalar ليحصل على شعاع خرج ويرسله للمهمة الاب – سيساهم هذا الشعاع في النتيجة النهائية بعد جمعه مع ما يقابله من أشعة الخرج المتبقية للمهمات الابناء

5- يستقبل الأب شعاع من الابن (بعده مساوٍ لاحدى بعدي المصوفة) ويقوم الاب للحصول على النتيجة النهائية بجمع (من دون ترتيب) الأشعة المستقبلة من الأبناء واحدا تلو اﻷخر حتى ينتهي ارسال جميع الابناء ( ويتم الجمع باستخدام التابع vectorSum)
 فيكون الاب قد  على شعاع وحيد يحوي النتيجة النهائية لعملية الضرب
 

                                                   PVM Multi Row

يأخذ الأب إضافة إلى ما يأخذه في الإجرائيات السابقة (بعدي المصفوفة و بعد الشعاع) متغير يخص عدد الابناء المطلوب إنشائهم ليتعاونوا على إجراء عملية ضرب المصفوفة بالشعاع

1-تقوم المهمة اﻷب بقراءة بيانات مصفوفة الدخل وشعاع الدخل من الملفات             (readArray.txt, readVector.txt) باستخدام التابع ReadData الموجود في الملف supportingArrays.h

![](https://github.com/aloshlouay/PVM_Project/blob/master/pictures/pvmmultirow1.png)

2-يقوم الأب أولا بحساب مايحتاجه إلى اسطر زائدة يجب إضافتها إلى المصفوفة ليتمكن من ارسال عدد متساوٍ من الاسطر لكل مهمة ابن (فلا تستقبل مهمة ابن عدد من الاسطر أكثر او أقل من بقية المهمات) يقوم بعدها بإنشاء متغير size يحوي عدد الاسطر التي سترسل لكل مهمة ابن
ويقوم بتوسيع المصفوفة الحالية لمصفوفة أكبر (إن تطلب اﻷمر) تحوي أسطر جديدة عناصرها تساوي الصفر باستعمال التابع expandRows_2DArray

3-تنشئ المهمة الاب عدد من الابناء مساوٍ لما تم طلبه فهذه المهمة جاهزة لتقسيم المصفوفة باستعمال slice_2DArray وإرسال كل قسم من المصفوفة المجزئة للابناء 
(ملاحظة: يرسل الاب المصفوفة المجزئة بعد تحويلها لمصفوفة من بعد واحد باستعمال التابع _2Dto1DArray )
ويرسل الاب إضافة الى المصفوفة المجزئة شعاع الدخل و المتغير size

![](https://github.com/aloshlouay/PVM_Project/blob/master/pictures/pvmmultirow2.png)

4-تستقبل كل مهمة ابن المصفوفة المجزئة و شعاع الدخل والمتغيرsize
وتقوم بضرب المصفوفة المجزئة بالشعاع باستخدام MatVect ويحصل الابن على شعاع خرج بُعده مساوٍ للمتغير size فيرسل الابن هذا الشعاع للاب

![](https://github.com/aloshlouay/PVM_Project/blob/master/pictures/pvmmultirow3.png)

5-يستقبل الاب الاشعة المختلفة من الابناء حيث أن بُعد جميع هذه الاشعة مساوٍ للمتغير size
حيث يقوم الاب بالاستقبال من الابناء من دون اي ترتيب الى ان يفرغ جميع الابناء من الارسال حيث يقوم  الاب بعد كل استقبال بوضع الشعاع المستقبل في مكانه الصحيح ضمن شعاع الخرج النهائي باستخدام التابعplace_vector
فبعد الاستقبال من جميع الابناء يكون كل ابن قد ساهم في حساب جزء من شعاع الخرج

![](https://github.com/aloshlouay/PVM_Project/blob/master/pictures/pvmmultirow4.png)


                                                   PVM Multi Col
  
يأخذ الأب إضافة إلى ما يأخذه في الإجرائيات السابقة (بعدي المصفوفة و بعد الشعاع) متغير يخص عدد الابناء المطلوب إنشائهم ليتعاونوا على إجراء عملية ضرب المصفوفة بالشعاع


1-تقوم المهمة اﻷب بقراءة بيانات مصفوفة الدخل وشعاع الدخل من الملفات             (readArray.txt, readVector.txt) باستخدام التابع ReadData الموجود في الملف supportingArrays.h

![](https://github.com/aloshlouay/PVM_Project/blob/master/pictures/pvmmulticol1.png)

2-يقوم الأب أولا بحساب مايحتاجه إلى أعمدة زائدة يجب إضافتها إلى المصفوفة ليتمكن من ارسال عدد متساوٍ من الاعمدة لكل مهمة ابن (فلا تستقبل مهمة ابن عدد من الاعمدة أكثر او أقل من بقية المهمات) يقوم بعدها بإنشاء متغير size يحوي عدد الاعمدة التي سترسل لكل مهمة ابن ويقوم بتوسيع المصفوفة الحالية لمصفوفة أكبر (إن تطلب اﻷمر) تحوي أعمدة جديدة عناصرها تساوي الصفر باستعمال expandRows_2DArray

3-تنشئ المهمة الاب عدد من الابناء مساوٍ لما تم طلبه فهذه المهمة جاهزة لتقسيم المصفوفة باستعمال slice_2DArray وإرسال كل قسم من المصفوفة المجزئة للابناء 
(ملاحظة: يرسل الاب المصفوفة المجزئة بعد تحويلها لمصفوفة من بعد واحد باستعمال التابع2Dto1DArray) ويرسل الاب إضافة الى المصفوفة المجزئة جزء من
 شعاع الدخل (حجم هذا الجزء مساوٍ ايضا للمتغير size)و المتغير size


![](https://github.com/aloshlouay/PVM_Project/blob/master/pictures/pvmmulticol2.png)

![](https://github.com/aloshlouay/PVM_Project/blob/master/pictures/pvmmulticol3.png)



4-تستقبل كل مهمة ابن المصفوفة المجزئة و شعاع الدخل والمتغيرsizeوتقوم بضرب المصفوفة المجزئة بالشعاع باستخدامMatVect ويحصل الابن على شعاع خرج بُعده مساوٍ لبُعد مصفوفة الدخل اﻷصلية فيرسل الابن هذا الشعاع للاب حيث يساهم كل شعاع في حساب الشعاع الناتج الاخير

![](https://github.com/aloshlouay/PVM_Project/blob/master/pictures/pvmmulticol4.png)


5-يستقبل الاب الاشعة المختلفة من الابناء حيث أن بُعد جميع هذه الاشعة مساوٍ لبُعد مصفوفة الدخل
فيقوم الاب بالاستقبال من الابناء من دون اي ترتيب الى ان يفرغ جميع الابناء من الارسال حيث يقوم  الاب بعد كل استقبال بجمع الشعاع المستقبل الى شعاع الخرج النهائي باستعمال التابع vectorSum
 الى ان ينتهي الارسال ويكون شعاع الخرج قد أصبح جاهزا

![](https://github.com/aloshlouay/PVM_Project/blob/master/pictures/pvmmulticol5.png)


# والحمد للّه ربّ العالمين
</div>
