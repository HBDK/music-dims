using System.Text.Json;
using System.Text.Json.Serialization;
using MusicDimsApi.Clients;
using MusicDimsApi.Endpoints.GetRadioChannels.Models;
using MusicDimsApi.Endpoints.Models;

namespace MusicDimsApi.Endpoints.GetRadioChannels.Gateways;

public class HaGateway(IHttpClientFactory clientFactory, ILogger<HaGateway> logger) : IHaGateway
{
    private const string GetLibraryRoute = "/api/services/music_assistant/get_library?return_response=true";

    public async Task<IEnumerable<NavigationEntityDto>> GetRadio()
    {
        var body = new GetRadioPostBody("01JNY60SY4J2FG3YE9TCBA6GK0", "radio", 500);
        var client = clientFactory.CreateClient(HomeAssistantClient.Name);
        var response = await client.PostAsync(GetLibraryRoute, new StringContent(JsonSerializer.Serialize(body), System.Text.Encoding.UTF8, "application/json"));

        response.EnsureSuccessStatusCode();

        var rawResult = await response.Content.ReadFromJsonAsync<RadioLibraryResponse>();
        return rawResult?.ServiceResponse.Items.Select(x => new NavigationEntityDto(x.Uri.Split("/").LastOrDefault() ?? string.Empty, x.Name, $"player:radio:{x.Uri.Split("/").LastOrDefault()}")).Where(x => !string.IsNullOrWhiteSpace(x.Id)) ?? [];
    }
}

public record GetRadioPostBody([property: JsonPropertyName("config_entry_id")]string ConfigEntry, [property: JsonPropertyName("media_type")]string MediaType, [property: JsonPropertyName("limit")] int Limit);
public interface IHaGateway
{
    Task<IEnumerable<NavigationEntityDto>> GetRadio();
}