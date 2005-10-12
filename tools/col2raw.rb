buf = $stdin.read()
buf = buf.gsub(/[ \t\n]/, '');

f = File.new("colmap.raw", "w")
buf.each_byte(){|i| f.write((i-?0).chr)}
f.close()

puts "Done"

# EOF #
