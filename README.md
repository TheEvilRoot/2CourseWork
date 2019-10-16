# Coursework #2
## English Proficiency Test

[![Build Status](https://travis-ci.org/TheEvilRoot/2CourseWork.svg?branch=master)](https://travis-ci.org/TheEvilRoot/2CourseWork)

Windows requirements
----

* Executable file
* Qt5Widgets.dll
* Qt5Core.dll
* Qt5Gui.dll
* libgcc_s_seh.dll
* libwinpthread-1.dll
* libstdc++-6.dll

Bugs to sovle and to-do list:
-----

* History loading (In some cases (UB for now) history entries for session states displaying as empty cells. 
* Sometimes segfault on click on history session state
* Windows encoding for custom strings for labels and table's headers
* Large answers not fit in ChoiceTest view in buttons
* Fullscreen adaptive layout
* CEFR result and result string not displaying on Result screen
* Session time not displaying in history and result screen (calculate as sum of times of all tests)
* Fix data set!!
* One of sentences has not missing word
* Button hover color should be darker!
* More noticeable message about test result (correct or wrong)
	* Maybe colorize to red and green
	* Or create floating message over question with disappear delay
	* Modal dialog (Mhhmhmhmmhmhmhmhmhmhmhmhmhmhhhh, I don't like it)
* Some sort of help messages to provide user what he can do on the screen
* Add more articles!!


Ideas
----

* Remove key-value word store
* Replace it with [key, key, key]-[value, value] word store system
	* Every word could have multiple translations and all of them can be submited as correct answer
	* Map<array<Russian>, array<English>>
		* map.getRussian(English) -> array<Russian>
		* map.getEnglish(Russian) -> array<English>
		* map.getWords(English|Russian> -> array<English|Russan>
		* map.check(English|Russian, Russian|English) -> bool
			* Check if first parameter has such related array that contains second parameter
		* map.randomPair() -> pair<English|Russian, Russian|English>
			* Get random pair of arrays in map
			* Get random word from first array
			* Get random word from second array
			* Return pair of theese words
