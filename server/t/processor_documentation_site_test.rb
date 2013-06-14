require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class ProcessorDocumentationSiteTest < Test::Unit::TestCase

  def setup
    @browser = Celerity::Browser.new
  end

  def teardown
    @browser.close
  end

  def test_selected_processor_documentation_sites
    processors = %w{ txt-reader tp-tokenizer psi-writer }

    processors.each do |processor|
      @browser.goto "#{$server_address}/help/processor.psis?name=#{processor}"

      header = @browser.div(:id => 'content').h1s.first.text.downcase
      assert !header.include?("not found"), "page for processor `#{processor}` not found"
    end
  end

  def test_selected_alias_documentation_sites
    aliases = %w{ lemmatizer parser }

    aliases.each do |aliass|
      @browser.goto "#{$server_address}/help/processor.psis?name=#{aliass}"

      header = @browser.div(:id => 'content').h1s.first.text.downcase
      assert !header.include?("not found"), "page for alias `#{aliass}` not found"
    end
  end

end
