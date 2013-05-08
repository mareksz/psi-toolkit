require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class PsiServerTest < Test::Unit::TestCase

  MIN_NUMBER_OF_ALIASES = 25

  def setup
    @browser = Celerity::Browser.new
  end

  def teardown
    @browser.close
  end

  def test_server_address
    assert_nothing_raised(NativeException) do
      @browser.goto $server_address
      @browser.close
    end
  end

  def test_list_of_publications_exists
    assert_nothing_raised(NativeException) do
      @browser.goto $server_address + '/publications.html'
    end

    assert @browser.ul(:id => 'publications').exists?
  end

  def test_list_of_aliases_works
    assert_nothing_raised(NativeException) do
      @browser.goto $server_address + '/help/aliases.html'
    end

    all_text = @browser.div(:id => 'content').text.downcase
    assert all_text.include? 'aliases'
    assert all_text.include? 'lemmatize'
    assert all_text.include? 'tokenize'

    number_of_aliases = @browser.divs.count do |div|
      div.attribute_value(:class) == 'alias-item'
    end
    assert number_of_aliases > MIN_NUMBER_OF_ALIASES
  end

  def test_download_site_works
    assert_nothing_raised(NativeException) do
      @browser.goto $server_address + '/download.html'
    end

    @browser.div(:id => 'downloads').uls.each do |ul|
      ul.as.each { |link| assert link.href.start_with?('http://mrt.wmi.amu.edu.pl:8080/job') }
    end
  end

  def test_bindings_site_works
    assert_nothing_raised(NativeException) do
      @browser.goto $server_address + '/bindings.html'
    end

    assert @browser.h2s.map(&:text).join(' ').include? 'Perl'
    assert @browser.h2s.map(&:text).join(' ').include? 'Python'
    assert @browser.h2s.map(&:text).join(' ').include? 'Ruby'
  end

  def test_email_presence_on_contact_site
    assert_nothing_raised(NativeException) do
      @browser.goto $server_address + '/contact.html'
    end

    assert @browser.div(:id => 'content').text.include? 'psi-toolkit@amu.edu.pl'
  end

end
