require 'webrick'

Thread.abort_on_exception = true

root = File.expand_path File.dirname(__FILE__), 'public_html'
server = WEBrick::HTTPServer.new :Port => 8000,
                                 :DocumentRoot => root

server.mount_proc '/stream.json' do |req, res|
  rd, wr = IO.pipe
  Thread.new {
    loop do
      begin
        wr.puts "data: [#{rand(100)}, #{rand(100)}]\n\n"
      rescue
        wr.close
        break
      end
      sleep 1
    end
  }
  res['Content-Type'] = 'text/event-stream'
  res.body = rd
  res.chunked = true
end

trap 'INT' do server.shutdown end

server.start
