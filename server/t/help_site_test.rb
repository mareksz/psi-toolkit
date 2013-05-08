require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class HelpSiteTest < Test::Unit::TestCase

    def setup
      @browser = Celerity::Browser.new
      @browser.goto $server_address + '/help/documentation.html'
    end

    def teardown
      @browser.close
    end

    def test_help_container_exists
      assert @browser.div(:class, 'help-item').exists?
    end

    def test_each_processor_has_title_and_description
      titles = @browser.div(:id => 'content').h2s
      assert !titles.empty?

      descriptions = divs_with_class('help-desc')
      assert !descriptions.empty?

      assert_equal titles.size, descriptions.size
    end

    def test_some_processors_have_option_list
      options = divs_with_class('help-opts')
      assert !options.empty?
    end

    def test_some_processors_have_language_list
      langs = divs_with_class('help-langs')
      assert !langs.empty?
    end

    def test_some_processors_have_aliases
      aliases = divs_with_class('help-alias')
      assert !aliases.empty?
    end

    private

    def divs_with_class(class_name)
      @browser.divs.select{ |div| div.attribute_value(:class) == class_name }
    end

end
