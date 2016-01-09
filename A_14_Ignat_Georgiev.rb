require 'green_shoes'
 Shoes.app title: "Ignat Georgiev's WordCounter",width:300,height:450 do
 	@filename = edit_line
 	fileame = ""
 	@filename.style(width:300)
 	@button1 = button("Browse file :")
 	@button3 = button ("Browse folder :")
 	@button2 = button("WC")
 	@button2.style(width:300)
 	@button1.style(width:300)
 	@button3.style(width:300)
 	@button1.click(){@filename.text = ask_open_file}
	@button2.click(){system("ruby /home/ignat/Desktop/software_engineering_2015/hm_count_words/A_14_Ignat_Georgiev.rb "  +@filename.text+ " csv")}
	@button3.click(){@filename.text = "-d "+ ask_open_folder}
end