require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class TutorialSiteTest < Test::Unit::TestCase

  def setup
    @browser = Celerity::Browser.new
    @browser.goto $server_address + '/help/tutorial.html'
  end

  def teardown
    @browser.close
  end

  def test_all_links_in_tutorial_works
    links = []
    @browser.div(:id => 'content').ps.each do |p|
      links += p.links.select{ |link| !link_with_runable_example?(link) }
    end
    links.compact!

    assert !links.empty?

    links.each do |link|
      assert_not_equal link.click_and_attach.status_code, 404,
        "link #{link} returns Error 404"
    end
  end

  def test_links_with_runable_examples_in_tutorial_works
    links = []
    @browser.div(:id => 'content').ps.each do |p|
      links += p.links.select{ |link| link_with_runable_example?(link) }
    end
    links.compact!

    assert !links.empty?

    links.each do |link|
      psi = link.click_and_attach
      assert_not_equal psi.status_code, 404,
        "link [#{link}] returns Error 404"
      assert !psi.text.downcase.include?('there are some problems'),
        "example [#{link}] is not working," +
        "getting page: [#{psi.div(:id => 'content').text}]"
    end
  end

  private

  def link_with_runable_example?(link)
    link.href.to_s.include?('pipe.psis?')
  end

end
