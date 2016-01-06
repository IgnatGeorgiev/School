require 'csv'
def folder(adress)
  files = Dir.glob("#{adress}/**/*").select { |f| File.file? f }

  files.each do |file|
    find_lines file
  end
end
def find_lines filename
	ruby = 0
	java = 0
	cpp = 0
	c = 0
	line_count = `wc -l "#{filename}"`.strip.split(' ')[0].to_i
	if filename.split("/").last.split(".").last == "cc" || filename.split("/").last.split(".").last == "cpp"
	cpp += line_count
	elsif filename.split("/").last.split(".").last == "rb"
	ruby += line_count
	elsif filename.split("/").last.split(".").last == "java"
	java +=line_count
	elsif filename.split("/").last.split(".").last == "c"
	c +=line_count	
	end
	puts "C++, #{cpp}"
	puts "Ruby, #{ruby}"
	puts "Java, #{java}"
	puts "C, #{c}"

end
folder ARGV[0]
