require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class PipeSiteTest < Test::Unit::TestCase

    def setup
        @browser = Celerity::Browser.new(:browser => :firefox)
        @browser.goto $server_address
    end

    def teardown
        @browser.close
    end

    def test_if_pipe_is_not_empty
        assert !@browser.text_field(:name => 'pipe-text').value.empty?
    end

    def test_bookmarks_before_switching
        assert @browser.div(:id, 'input-text').visible?
        assert !@browser.div(:id, 'input-file').visible?

        active_li_class = @browser.a(:class, 'input-text').parent.attribute_value(:class)
        assert active_li_class.include?('active')

        inactive_li_class = @browser.a(:class, 'input-file').parent.attribute_value(:class)
        assert !inactive_li_class.include?('active')
    end

    def test_bookmarks_after_switching
        @browser.a(:class => 'input-file').click

        assert !@browser.div(:id, 'input-text').visible?
        assert @browser.div(:id, 'input-file').visible?

        inactive_li_class = @browser.a(:class, 'input-text').parent.attribute_value(:class)
        assert !inactive_li_class.include?('active')

        active_li_class = @browser.a(:class, 'input-file').parent.attribute_value(:class)
        assert active_li_class.include?('active')
    end

    def test_if_submitting_of_an_input_text_works
        input  = @browser.text_field(:name => 'input-text')
        assert input.exists?
        assert !input.value.empty?

        txt = 'Marysia ma rysia'

        # set an input text
        assert_not_equal input.value, txt
        input.set txt
        assert input.value, txt

        # click a submit button
        btn = @browser.button(:name => 'pipe-submit')
        assert btn.exists?
        btn.click

        # check if pipe output contains expected words
        txt.split.each do |word|
            assert @browser.text.include? word
        end
    end

    def test_if_submitting_of_an_input_file_works
        input_file = @browser.file_field(:name => 'input-file')
        assert input_file.exists?

        file_name = "file.txt"
        txt = 'Zosia nie ma nic'

        # create file to test
        File.open(file_name, "w") { |f| f.puts txt }
        input_file.set file_name
        assert input_file.value, txt

        # switch to file bookmark
        @browser.a(:class => 'input-file').click

        submit

        # check if pipe output contains all words
        txt.split.each do |word|
           assert @browser.pre.text.include? word
        end

        File.delete file_name
    end

    def test_if_element_to_download_exists
        @browser.div(:id => 'download').exists?
    end

    def test_if_downloading_a_file_with_output_works
        set_input 'Marysia ma rysia'
        submit

        # compare pipe output and generated file content
        output = @browser.pre.text

        file = @browser.div(:id => 'download').link.download
        file_content = file.readlines.join
        file_content.gsub!(/(\n| )+/, ' ')
        file_content.strip!

        assert_equal output, file_content
    end

    def test_picture_output
        set_pipe 'gv-writer --format jpg'
        set_input 'Ala ma kota i psa.'
        submit

        output = @browser.div(:id => 'output')

        assert output.image.exists?
        assert output.image.src.end_with? 'jpg'

        assert output.link.exists?
        assert output.link.href.end_with? 'jpg'
    end

    private

    def set_pipe(txt)
        pipe = @browser.text_field(:name => 'pipe-text')
        pipe.set txt
        return pipe
    end

    def set_input(txt)
        input = @browser.text_field(:name => 'input-text')
        input.set txt
        return input
    end

    def submit
        btn = @browser.button(:name => 'pipe-submit')
        btn.click
    end

end
