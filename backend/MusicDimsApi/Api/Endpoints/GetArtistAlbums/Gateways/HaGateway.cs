using System.Text.Json;
using System.Text.Json.Serialization;
using MusicDimsApi.Clients;
using MusicDimsApi.Endpoints.GetArtistAlbums.Models;
using MusicDimsApi.Endpoints.GetArtists.Models;

namespace MusicDimsApi.Endpoints.GetArtistAlbums.Gateways;

public class HaGateway(IHttpClientFactory clientFactory, ILogger<HaGateway> logger) : IHaGateway
{
    private const string GetLibraryRoute = "/api/services/music_assistant/search?return_response=true";

    public async Task<IEnumerable<AlbumResponseDto>> GetAlbums(string id)
    {
        var name = id.Replace("_", " ");
        var body = new SearchPostBody("01JNY60SY4J2FG3YE9TCBA6GK0", ["album"], name, " ", true);
        var client = clientFactory.CreateClient(HomeAssistantClient.Name);
        var response = await client.PostAsync(GetLibraryRoute, new StringContent(JsonSerializer.Serialize(body), System.Text.Encoding.UTF8, "application/json"));

        response.EnsureSuccessStatusCode();

        var rawResult = await response.Content.ReadFromJsonAsync<MusicAssistantLibraryResponse>();
        return rawResult?.ServiceResponse.Albums.Select(x => new AlbumResponseDto(x.Uri.Split("/").LastOrDefault() ?? "", x.Name, $"player:album:{x.Uri.Split("/").LastOrDefault() ?? String.Empty}")).Where(x => !string.IsNullOrWhiteSpace(x.Id)) ?? [];
    }
}

public record SearchPostBody([property: JsonPropertyName("config_entry_id")]string ConfigEntry, [property: JsonPropertyName("media_type")]string[] MediaType, [property: JsonPropertyName("artist")] string Artist, [property: JsonPropertyName("name")] string Name, [property: JsonPropertyName("library_only")] bool LibraryOnly);
public interface IHaGateway
{
    Task<IEnumerable<AlbumResponseDto>> GetAlbums(string id);
}