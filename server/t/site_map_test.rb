require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class SiteMapTest < Test::Unit::TestCase

  def setup
    @browser = Celerity::Browser.new
    @browser.goto $server_address + '/sitemap.html'
  end

  def teardown
    @browser.close
  end

  PAGES_TO_SKIP = %w{ Github PSI-Blog }

  def test_all_links_on_site_map_works
    links = @browser.ul(:id => 'sitemap').links
    assert !links.empty?

    links.each_with_index do |link, idx|
      next if PAGES_TO_SKIP.include?(link.text)
      site = nil
      assert_nothing_raised { site = link.click_and_attach }
      assert_not_equal site.status_code, 404, "link #{link} returns Error 404"
    end
  end

end
