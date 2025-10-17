using System.Net;
using System.Text.Json;
using System.Text.Json.Serialization;
using MusicDimsApi.Clients;
using MusicDimsApi.Endpoints.GetArtists.Models;
using MusicDimsApi.Endpoints.Models;

namespace MusicDimsApi.Endpoints.GetArtists.Gateways;

public class HaGateway(IHttpClientFactory clientFactory, ILogger<HaGateway> logger) : IHaGateway
{
    private const string GetLibraryRoute = "/api/services/music_assistant/get_library?return_response=true";

    public async Task<IEnumerable<NavigationEntityDto>> GetLibrary()
    {
        var body = new GetLibraryPostBody("01JNY60SY4J2FG3YE9TCBA6GK0", "artist", true, 500);
        var client = clientFactory.CreateClient(HomeAssistantClient.Name);
        var response = await client.PostAsync(GetLibraryRoute, new StringContent(JsonSerializer.Serialize(body), System.Text.Encoding.UTF8, "application/json"));

        response.EnsureSuccessStatusCode();

        var rawResult = await response.Content.ReadFromJsonAsync<MusicAssistantGetLibraryResponse>();
        return rawResult?.ServiceResponse.Items.Select(x => new NavigationEntityDto(x.Name.Replace(" ", "_"), x.Name, $"/artists/{WebUtility.UrlEncode(x.Name.Replace(" ", "_")) }/albums")).Where(x => !string.IsNullOrWhiteSpace(x.Id)) ?? [];
    }
}

public record GetLibraryPostBody([property: JsonPropertyName("config_entry_id")]string ConfigEntry, [property: JsonPropertyName("media_type")]string MediaType, [property: JsonPropertyName("album_artists_only")] bool AlbumArtistOnly, [property: JsonPropertyName("limit")] int Limit);
public interface IHaGateway
{
    Task<IEnumerable<NavigationEntityDto>> GetLibrary();
}